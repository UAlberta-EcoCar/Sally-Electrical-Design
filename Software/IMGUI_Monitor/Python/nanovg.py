import glfw
import OpenGL.GL as gl
import imgui
import time
from imgui.integrations.glfw import GlfwRenderer
def draw_coil(draw_list, base_x, base_y, windings=6, spacing=8):
    """
    Draws a relay coil as multiple windings (small circles).
    base_x, base_y = top-left position
    windings = number of loops
    spacing = horizontal spacing between loops
    """
    color = imgui.get_color_u32_rgba(1, 1, 1, 1)
    radius = 6
    for i in range(windings):
        cx = base_x + i * spacing
        cy = base_y
        draw_list.add_circle(cx, cy, radius, color, 32, 2.0)

    # Coil terminals (vertical lines at ends)
    draw_list.add_line(base_x - spacing, base_y, base_x - spacing, base_y + 20, color, 2.0)
    draw_list.add_line(base_x + windings * spacing, base_y, base_x + windings * spacing, base_y + 20, color, 2.0)

    # Label
    draw_list.add_text(base_x, base_y - 25, color, "Coil")


def draw_spdt_relay(draw_list, base_x, base_y, coil_active=False, flowing_current=False, anim_phase=0.0):
    """
    Draws a SPDT relay schematic symbol with animation.
    coil_active=True means coil energized (COM->NO).
    flowing_current=True means animate yellow boxes along the active wire.
    anim_phase is a float that increments each frame for animation.
    """


    # --- Common terminal ---
    common_x, common_y = base_x + 150, base_y + 60
    draw_list.add_circle_filled(common_x, common_y, 5, imgui.get_color_u32_rgba(1,1,0,1))
    draw_list.add_text(common_x - 20, common_y + 15, imgui.get_color_u32_rgba(1,1,1,1), "COM")


    # In your relay drawing function:
    coil_x = common_x - 40
    coil_y = common_y + 80
    draw_coil(draw_list, coil_x, coil_y, windings=6, spacing=10)

    # --- NC terminal ---
    nc_x, nc_y = base_x + 250, base_y + 30
    draw_list.add_circle_filled(nc_x, nc_y, 5, imgui.get_color_u32_rgba(0,1,0,1))
    draw_list.add_text(nc_x - 20, nc_y - 20, imgui.get_color_u32_rgba(1,1,1,1), "NC")

    # --- NO terminal ---
    no_x, no_y = base_x + 250, base_y + 90
    draw_list.add_circle_filled(no_x, no_y, 5, imgui.get_color_u32_rgba(0,1,0,1))
    draw_list.add_text(no_x - 20, no_y + 15, imgui.get_color_u32_rgba(1,1,1,1), "NO")

    # --- Coil to COM line ---
    draw_list.add_line(coil_x+20, coil_y, common_x-5, common_y, imgui.get_color_u32_rgba(1,1,1,1), 2.0)

    # --- COM to NC/NO depending on coil state ---
    if coil_active:
        # COM -> NO
        draw_list.add_line(common_x+5, common_y, no_x-5, no_y, imgui.get_color_u32_rgba(1,1,1,1), 2.0)
        if flowing_current:
            # Animate yellow boxes along COM->NO
            for i in range(5):
                offset = (anim_phase*20 + i*40) % (no_x-common_x-10)
                bx = common_x+5+offset
                by = common_y + (no_y-common_y)*offset/(no_x-common_x-10)
                draw_list.add_rect_filled(int(bx), int(by-3), int(bx+8), int(by+3),
                                          imgui.get_color_u32_rgba(1,1,0,1))
    else:
        # COM -> NC
        draw_list.add_line(common_x+5, common_y, nc_x-5, nc_y, imgui.get_color_u32_rgba(1,1,1,1), 2.0)
        if flowing_current:
            # Animate yellow boxes along COM->NC
            for i in range(5):
                offset = (anim_phase*20 + i*40) % (nc_x-common_x-10)
                bx = common_x+5+offset
                by = common_y + (nc_y-common_y)*offset/(nc_x-common_x-10)
                draw_list.add_rect_filled(int(bx), int(by-3), int(bx+8), int(by+3),
                                          imgui.get_color_u32_rgba(1,1,0,1))


def main():
    if not glfw.init():
        print("Could not initialize GLFW")
        return

    window = glfw.create_window(800, 600, "SPDT Relay Animated Demo", None, None)
    glfw.make_context_current(window)

    imgui.create_context()
    impl = GlfwRenderer(window)

    coil_active = False
    flowing_current = False
    start_time = time.time()

    while not glfw.window_should_close(window):
        glfw.poll_events()
        impl.process_inputs()

        imgui.new_frame()

        # UI controls
        imgui.begin("Relay Control")
        changed, coil_active = imgui.checkbox("Coil Energized (COM->NO)", coil_active)
        changed, flowing_current = imgui.checkbox("Flowing Current", flowing_current)
        imgui.text(f"Coil current: {'ON' if coil_active else 'OFF'}")
        imgui.text(f"Flowing current: {'ON' if flowing_current else 'OFF'}")

        # Draw relay symbol
        draw_list = imgui.get_window_draw_list()
        base_x, base_y = imgui.get_cursor_screen_pos()
        anim_phase = (time.time() - start_time)
        draw_spdt_relay(draw_list, base_x, base_y, coil_active, flowing_current, anim_phase)
        imgui.invisible_button("canvas", 400, 150)
        imgui.end()

        # Render
        imgui.render()
        gl.glClearColor(0.1, 0.1, 0.1, 1)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)
        impl.render(imgui.get_draw_data())
        glfw.swap_buffers(window)

    impl.shutdown()
    glfw.terminate()


if __name__ == "__main__":
    main()
