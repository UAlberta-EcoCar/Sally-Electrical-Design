from imgui_bundle import hello_imgui, immapp, imgui
# import imgui_bundle.imgui as imgui

def draw_logs(app):
    imgui.begin("Logs", True)
    imgui.text("Status:")
    imgui.text("Status:")
    imgui.text("Status:")
    imgui.begin_child("stat_child", imgui.ImVec2(0, 120))
    # imgui.begin_child("status_child", 0, 120, True)
    for line in app.status_log[-300:]:
        imgui.text_unformatted(line)
    if app.auto_scroll_logs: imgui.set_scroll_here_y(1.0)
    imgui.end_child()

    imgui.text("Errors:")
    imgui.begin_child("err_child", imgui.ImVec2(0, 120))
    # imgui.begin_child("errors_child", 0, 100, True)
    for line in app.errors_log[-200:]:
        imgui.text_colored(imgui.ImVec4(0.9, 0.4, 0.3, 1.0), line)
    if app.auto_scroll_logs: imgui.set_scroll_here_y(1.0)
    imgui.end_child()

    if app.show_raw_log:
        imgui.text("Raw UART:")
        # imgui.begin_child("raw_child", 0, 0, True)
        imgui.begin_child("raw_child", imgui.ImVec2(0, 120))
        for line in app.recent_lines[-1000:]:
            imgui.text_unformatted(line)
        if app.auto_scroll_logs: imgui.set_scroll_here_y(1.0)
        imgui.end_child()
    imgui.end()
