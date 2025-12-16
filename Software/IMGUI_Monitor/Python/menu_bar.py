import imgui

def draw_menu_bar(app):
    if imgui.begin_main_menu_bar():
        if imgui.begin_menu("Serial", True):
            if imgui.menu_item("Refresh ports")[0]:
                app.refresh_ports()
            if not app.connected:
                if imgui.menu_item("Connect")[0]:
                    app.connect_requested = True
            else:
                if imgui.menu_item("Disconnect")[0]:
                    app.disconnect_requested = True
            imgui.end_menu()

        if imgui.begin_menu("View", True):
            _, app.show_raw_log = imgui.menu_item("Show raw log", None, app.show_raw_log, True)
            _, app.show_parsed_panel = imgui.menu_item("Show parsed panel", None, app.show_parsed_panel, True)
            _, app.auto_scroll_logs = imgui.menu_item("Auto-scroll logs", None, app.auto_scroll_logs, True)
            imgui.end_menu()
        imgui.end_main_menu_bar()
