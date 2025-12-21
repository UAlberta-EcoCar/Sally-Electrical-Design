from imgui_bundle import imgui

def draw_parsed(app):
    if not app.show_parsed_panel: return
    imgui.begin("Parsed telemetry", True)
    for k in sorted(app.parsed_latest.keys()):
        v = app.parsed_latest[k]
        imgui.columns(2)
        imgui.text(k)
        imgui.text(k)
        imgui.next_column()
        imgui.text(str(v))
        imgui.columns(1)
        imgui.separator()
    imgui.end()
