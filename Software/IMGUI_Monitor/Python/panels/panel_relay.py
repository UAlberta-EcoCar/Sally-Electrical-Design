from imgui_bundle import imgui

def draw_spdt_relay(app_state):
    imgui.begin("SPDT Relay Symbol")

    draw_list = imgui.get_window_draw_list()
    base_x, base_y = imgui.get_cursor_screen_pos()

    # Coil (circle on the left)
    coil_x, coil_y = base_x + 50, base_y + 60
    draw_list.add_circle(coil_x, coil_y, 20, imgui.get_color_u32(imgui.ImVec4(1, 1, 1, 1)), 32, 2.0)
    draw_list.add_text(coil_x - 15, coil_y - 35, imgui.get_color_u32(1, 1, 1, 1), "Coil")

    # Common terminal
    common_x, common_y = base_x + 150, base_y + 60
    draw_list.add_circle_filled(common_x, common_y, 5, imgui.get_color_u32_rgba(1, 1, 0, 1))
    draw_list.add_text(common_x - 20, common_y + 15, imgui.get_color_u32_rgba(1, 1, 1, 1), "COM")

    # NC terminal (top)
    nc_x, nc_y = base_x + 250, base_y + 30
    draw_list.add_circle_filled(nc_x, nc_y, 5, imgui.get_color_u32_rgba(0, 1, 0, 1))
    draw_list.add_text(nc_x - 20, nc_y - 20, imgui.get_color_u32_rgba(1, 1, 1, 1), "NC")

    # NO terminal (bottom)
    no_x, no_y = base_x + 250, base_y + 90
    draw_list.add_circle_filled(no_x, no_y, 5, imgui.get_color_u32_rgba(0, 1, 0, 1))
    draw_list.add_text(no_x - 20, no_y + 15, imgui.get_color_u32_rgba(1, 1, 1, 1), "NO")

    # Draw lines: coil to common, common to NC/NO
    draw_list.add_line(coil_x + 20, coil_y, common_x - 5, common_y,
                       imgui.get_color_u32_rgba(1, 1, 1, 1), 2.0)

    # Common to NC
    draw_list.add_line(common_x + 5, common_y, nc_x - 5, nc_y,
                       imgui.get_color_u32_rgba(1, 1, 1, 1), 2.0)

    # Common to NO
    draw_list.add_line(common_x + 5, common_y, no_x - 5, no_y,
                       imgui.get_color_u32_rgba(1, 1, 1, 1), 2.0)

    imgui.end()
