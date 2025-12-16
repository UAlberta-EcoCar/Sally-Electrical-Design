import imgui

def draw_serial_control(app):
    imgui.begin("Serial Control", True)
    imgui.text("COM port:")
    imgui.text("COM port:")
    imgui.text("COM port:")
    imgui.text("COM port:")
    if imgui.button("Refresh"):
        app.refresh_ports()
    imgui.same_line()
    ports_display = app.available_ports if app.available_ports else ["<no ports>"]
    current = app.selected_port_idx if app.available_ports else 0
    changed, current = imgui.combo("##com_combo", current, ports_display)
    if changed and app.available_ports:
        app.selected_port_idx = current

    changed, br_str = imgui.input_text("Baudrate", str(app.baudrate), 16)
    if changed:
        try: app.baudrate = int(br_str)
        except ValueError: pass

    if not app.connected:
        if imgui.button("Connect"): app.connect_requested = True
    else:
        if imgui.button("Disconnect"): app.disconnect_requested = True

    imgui.separator()
    imgui.text_colored(("Connected" if app.connected else "Disconnected"),
                       0.2, 0.8 if app.connected else 0.2, 0.2, 1.0)
    imgui.end()
