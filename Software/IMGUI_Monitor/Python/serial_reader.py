import threading
import time
import serial
from serial import SerialException

class SerialReader(threading.Thread):
    def __init__(self, port, baudrate, line_queue, status_queue, stop_event,
                 retry_initial_delay=0.5, retry_max_delay=5.0):
        super().__init__(daemon=True)
        self.port = port
        self.baudrate = baudrate
        self.line_queue = line_queue
        self.status_queue = status_queue
        self.stop_event = stop_event
        self.retry_initial_delay = retry_initial_delay
        self.retry_max_delay = retry_max_delay
        self.ser = None

    def run(self):
        delay = self.retry_initial_delay
        while not self.stop_event.is_set():
            try:
                self.status_queue.put(("info", f"Connecting to {self.port} @ {self.baudrate}..."))
                self.ser = serial.Serial(self.port, self.baudrate, timeout=0.1)
                self.ser.reset_input_buffer()
                self.status_queue.put(("ok", f"Connected: {self.port}"))
                delay = self.retry_initial_delay

                buffer = b""
                while not self.stop_event.is_set():
                    try:
                        chunk = self.ser.read(1024)
                        if chunk:
                            buffer += chunk
                            while b"\n" in buffer or b"\r" in buffer:
                                buffer = buffer.replace(b"\r\n", b"\n").replace(b"\r", b"\n")
                                line, sep, rest = buffer.partition(b"\n")
                                buffer = rest
                                text = line.decode(errors="replace").strip()
                                if text:
                                    self.line_queue.put(text)
                        else:
                            time.sleep(0.01)
                    except SerialException as e:
                        self.status_queue.put(("error", f"Serial read error: {e}"))
                        break

                if self.stop_event.is_set():
                    break
                self._close_serial()
                self.status_queue.put(("warn", f"Disconnected. Retrying in {delay:.1f}s..."))
                time.sleep(delay)
                delay = min(self.retry_max_delay, delay * 1.7)

            except SerialException as e:
                self.status_queue.put(("error", f"Unable to connect: {e}. Retrying in {delay:.1f}s..."))
                time.sleep(delay)
                delay = min(self.retry_max_delay, delay * 1.7)
            except Exception as e:
                self.status_queue.put(("error", f"Unexpected error: {e}. Retrying in {delay:.1f}s..."))
                time.sleep(delay)
                delay = min(self.retry_max_delay, delay * 1.7)

        self._close_serial()
        self.status_queue.put(("info", "Reader thread stopped."))

    def _close_serial(self):
        try:
            if self.ser and self.ser.is_open:
                self.ser.close()
        except Exception:
            pass
        self.ser = None
