import queue, threading, time
from serial.tools import list_ports
from serial_reader import SerialReader
from parser import parse_line

class AppState:
    def __init__(self):
        self.available_ports = []
        self.selected_port_idx = 0
        self.baudrate = 115200
        self.connect_requested = False
        self.disconnect_requested = False
        self.connected = False

        self.serial_thread = None
        self.stop_event = threading.Event()
        self.line_queue = queue.Queue()
        self.status_queue = queue.Queue()

        self.recent_lines = []
        self.parsed_latest = {}
        self.errors_log = []
        self.status_log = []

        self.auto_scroll_logs = True
        self.show_raw_log = True
        self.show_parsed_panel = True

    def refresh_ports(self):
        ports = list_ports.comports()
        self.available_ports = [p.device for p in ports]
        if self.selected_port_idx >= len(self.available_ports):
            self.selected_port_idx = 0

    def start_reader(self):
        if self.serial_thread and self.serial_thread.is_alive():
            return
        if not self.available_ports:
            self.status_log.append("No COM ports available.")
            return
        port = self.available_ports[self.selected_port_idx]
        self.stop_event.clear()
        self.serial_thread = SerialReader(
            port=port,
            baudrate=self.baudrate,
            line_queue=self.line_queue,
            status_queue=self.status_queue,
            stop_event=self.stop_event,
        )
        self.serial_thread.start()
        self.connected = True

    def stop_reader(self):
        self.stop_event.set()
        if self.serial_thread:
            self.serial_thread.join(timeout=2.0)
        self.serial_thread = None
        self.connected = False

    def process_queues(self):
        while True:
            try:
                level, msg = self.status_queue.get_nowait()
                stamp = time.strftime("%H:%M:%S")
                self.status_log.append(f"[{stamp}] {level.upper()}: {msg}")
            except queue.Empty:
                break
        while True:
            try:
                line = self.line_queue.get_nowait()
                self.recent_lines.append(line)
                if len(self.recent_lines) > 1000:
                    self.recent_lines = self.recent_lines[-500:]
                status, parsed = parse_line(line)
                if status == "ok":
                    self.parsed_latest.update(parsed)
                else:
                    self.errors_log.append(f"Parse error: {parsed.get('error')} | raw: {parsed.get('raw')}")
            except queue.Empty:
                break
