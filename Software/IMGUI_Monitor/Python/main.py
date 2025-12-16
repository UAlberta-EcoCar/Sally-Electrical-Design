# import sys, os, importlib, inspect, glfw, imgui
# from imgui.integrations.glfw import GlfwRenderer
# from app_state import AppState
# import imgui
# import glfw
# import OpenGL.GL as gl
# from imgui.integrations.glfw import GlfwRenderer
# # --- Panel discovery ---
# def discover_panels():
#     panels = []
#     panels_dir = os.path.join(os.path.dirname(__file__), "panels")
#     sys.path.append(panels_dir)

#     for fname in os.listdir(panels_dir):
#         if fname.startswith("panel_") and fname.endswith(".py"):
#             modname = fname[:-3]  # strip .py
#             module = importlib.import_module(modname)
#             # Look for functions starting with "draw_"
#             for name, func in inspect.getmembers(module, inspect.isfunction):
#                 if name.startswith("draw_"):
#                     panels.append(func)
#     return panels

# def impl_glfw_init(window_name="minimal ImGui/GLFW3 example", width=1280, height=720):
#     if not glfw.init():
#         print("Could not initialize OpenGL context")
#         exit(1)

#     # OS X supports only forward-compatible core profiles from 3.2
#     glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
#     glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
#     glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

#     glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, gl.GL_TRUE)

#     # Create a windowed mode window and its OpenGL context
#     window = glfw.create_window(int(width), int(height), window_name, None, None)
#     glfw.make_context_current(window)

#     if not window:
#         glfw.terminate()
#         print("Could not initialize Window")
#         exit(1)

#     return window

# def main():
#     backgroundColor = (0, 0, 0, 1)
#     window = impl_glfw_init()
#     gl.glClearColor(*backgroundColor)
#     imgui.create_context()
#     impl = GlfwRenderer(window)

#     string = ""
#     f = 0.5


#     if not glfw.init():
#         print("Failed to init GLFW"); sys.exit(1)
#     window = glfw.create_window(1200, 800, "UART Monitor", None, None)
#     glfw.make_context_current(window)

#     imgui.create_context()
#     io = imgui.get_io()
#     #io.config_flags |= imgui.CONFIG_FLAGS_DOCKING_ENABLE
#     #io.config_flags |= imgui.CONFIG_FLAGS_VIEWPORTS_ENABLE
#     impl = GlfwRenderer(window)
#     imgui.style_colors_dark()

#     app = AppState()
#     app.refresh_ports()

#     # Discover panels dynamically
#     panels = discover_panels()

#     while not glfw.window_should_close(window):
#         glfw.poll_events()
#         impl.process_inputs()
#         # imgui.new_frame()

#         # Dockspace
#         # dockspace_flags = imgui.DOCK_NODE_FLAGS_NONE
#         # window_flags = imgui.WINDOW_FLAGS_MENU_BAR | imgui.WINDOW_FLAGS_NO_TITLE_BAR | \
#         #                imgui.WINDOW_FLAGS_NO_COLLAPSE | imgui.WINDOW_FLAGS_NO_RESIZE | \
#         #                imgui.WINDOW_FLAGS_NO_MOVE
#         # viewport = imgui.get_main_viewport()
#         # imgui.set_next_window_pos(viewport.pos.x, viewport.pos.y)
#         # imgui.set_next_window_size(viewport.size.x, viewport.size.y)
#         # imgui.set_next_window_viewport(viewport.id)
#         # imgui.push_style_var(imgui.STYLE_WINDOW_ROUNDING, 0.0)
#         # imgui.push_style_var(imgui.STYLE_WINDOW_BORDERSIZE, 0.0)
#         # imgui.begin("DockSpace", True, window_flags)
#         # imgui.pop_style_var(2)
#         # dock_id = imgui.get_id("MyDockSpace")
#         glfw.poll_events()
#         impl.process_inputs()
#         imgui.new_frame()
#         imgui.begin("Custom window", True)
#         imgui.show_test_window()

#         imgui.end()
#         imgui.render()
#         gl.glClearColor(*backgroundColor)
#         gl.glClear(gl.GL_COLOR_BUFFER_BIT)
#         impl.render(imgui.get_draw_data())
#         glfw.swap_buffers(window)

#     impl.shutdown()
#     glfw.terminate()

# main()

import sys, os, importlib, inspect
import glfw
import imgui
import OpenGL.GL as gl
from imgui.integrations.glfw import GlfwRenderer
from app_state import AppState
from panel_manager import PanelManager
import time

def discover_panels():
    panels = []
    panels_dir = os.path.join(os.path.dirname(__file__), "panels")
    sys.path.append(panels_dir)
    for fname in os.listdir(panels_dir):
        if fname.startswith("panel_") and fname.endswith(".py"):
            modname = fname[:-3]
            module = importlib.import_module(modname)
            for name, func in inspect.getmembers(module, inspect.isfunction):
                if name.startswith("draw_"):
                    panels.append(func)
    return panels

def impl_glfw_init(window_name="UART Monitor", width=1280, height=720):
    if not glfw.init():
        print("Could not initialize OpenGL context")
        sys.exit(1)
    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
    glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, gl.GL_TRUE)
    window = glfw.create_window(width, height, window_name, None, None)
    if not window:
        glfw.terminate()
        print("Could not initialize Window")
        sys.exit(1)
    glfw.make_context_current(window)
    return window

def main():
    backgroundColor = (0, 0, 0, 1)
    window = impl_glfw_init()
    gl.glClearColor(*backgroundColor)
    imgui.create_context()
    impl = GlfwRenderer(window)
    imgui.style_colors_dark()

    app = AppState()
    app.refresh_ports()
    panels = discover_panels()

    panels = PanelManager()
    panels.discover_panels()
    last_check = time.time()

    while not glfw.window_should_close(window):
        glfw.poll_events()
        impl.process_inputs()
        imgui.new_frame()

        # 🔄 Hot reload check every 2 seconds
        if time.time() - last_check > 0.1:
            panels.discover_panels()
            last_check = time.time()

        panels.render_panels(app)

        imgui.render()
        gl.glClearColor(*backgroundColor)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)
        impl.render(imgui.get_draw_data())
        glfw.swap_buffers(window)


    impl.shutdown()
    glfw.terminate()

if __name__ == "__main__":
    main()
