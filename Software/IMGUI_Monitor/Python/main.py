# import sys, os, importlib, inspect
# import glfw
# # import imgui
# import OpenGL.GL as gl
# from imgui.integrations.glfw import GlfwRenderer
# from app_state import AppState
# from panel_manager import PanelManager
# import time
# from imgui_bundle import imgui
# def discover_panels():
#     panels = []
#     panels_dir = os.path.join(os.path.dirname(__file__), "panels")
#     sys.path.append(panels_dir)
#     for fname in os.listdir(panels_dir):
#         if fname.startswith("panel_") and fname.endswith(".py"):
#             modname = fname[:-3]
#             module = importlib.import_module(modname)
#             for name, func in inspect.getmembers(module, inspect.isfunction):
#                 if name.startswith("draw_"):
#                     panels.append(func)
#     return panels

# def impl_glfw_init(window_name="UART Monitor", width=1280, height=720):
#     if not glfw.init():
#         print("Could not initialize OpenGL context")
#         sys.exit(1)
#     glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
#     glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
#     glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
#     glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, gl.GL_TRUE)
#     window = glfw.create_window(width, height, window_name, None, None)
#     if not window:
#         glfw.terminate()
#         print("Could not initialize Window")
#         sys.exit(1)
#     glfw.make_context_current(window)
#     return window

# def main():
#     window = impl_glfw_init()
#     imgui.create_context()
#     impl = GlfwRenderer(window)
#     imgui.style_colors_dark()

#     app = AppState()
#     panels = PanelManager()
#     panels.discover_panels()
#     last_check = time.time()

#     while not glfw.window_should_close(window):
#         glfw.poll_events()
#         impl.process_inputs()
#         imgui.new_frame()

#         if time.time() - last_check > 0.5:
#             panels.discover_panels()
#             last_check = time.time()

#         panels.render_panels(app)

#         imgui.render()
#         gl.glClearColor(0, 0, 0, 1)
#         gl.glClear(gl.GL_COLOR_BUFFER_BIT)
#         impl.render(imgui.get_draw_data())
#         glfw.swap_buffers(window)

#     impl.shutdown()
#     glfw.terminate()

# if __name__ == "__main__":
#     main()

from math import nan
from unittest import runner
from imgui_bundle import hello_imgui, immapp, imgui, nanovg
from app_state import AppState
import app_state
from panel_manager import PanelManager
import time


def create_docking_splits():
    # Split the main dockspace into left and bottom zones
    split_left = hello_imgui.DockingSplit()
    split_left.initial_dock = "MainDockSpace"
    split_left.new_dock = "LeftSpace"
    split_left.direction = imgui.Dir.left
    split_left.ratio = 0.25

    split_bottom = hello_imgui.DockingSplit()
    split_bottom.initial_dock = "MainDockSpace"
    split_bottom.new_dock = "BottomSpace"
    split_bottom.direction = imgui.Dir.down
    split_bottom.ratio = 0.3

    return [split_left, split_bottom]


def create_dockable_windows(app: AppState, panel_mgr: PanelManager):
    windows = []
    for func in panel_mgr.panel_funcs:
        w = hello_imgui.DockableWindow()
        w.label = func.__name__          # window title
        w.dock_space_name = "MainDockSpace"  # all panels go into the global dockspace
        w.gui_function = lambda f=func: f(app)
        windows.append(w)
    return windows

def main():
    app = AppState()
    panels = PanelManager("panels")
    panels.discover_panels()
    last_check = time.time()

    runner_params = hello_imgui.RunnerParams()
    runner_params.app_window_params.window_title = "UART Monitor"
    runner_params.docking_params.docking_splits = create_docking_splits()
     # First, tell HelloImGui that we want full screen dock space (this will create "MainDockSpace")
    runner_params.imgui_window_params.default_imgui_window_type = (
        hello_imgui.DefaultImGuiWindowType.provide_full_screen_dock_space
    )
    runner_params.imgui_window_params.enable_viewports = True
    # runner_params.imgui_window_params.
    runner_params.docking_params.dockable_windows = create_dockable_windows(app, panels)
    # runner_params.docking_params.doc
    
    runner_params.docking_params.main_dock_space_node_flags = (
        imgui.DockNodeFlags_.passthru_central_node  # makes the dockspace cover the full window
    )
    # runner_params.app_window_params.
    # runner_params.app_window_params.enable_multi_viewport = True

    def show_gui():

        nonlocal last_check
        if time.time() - last_check > 0.5:
            panels.discover_panels()
            runner_params.docking_params.dockable_windows = create_dockable_windows(app, panels)
            last_check = time.time()
        # app.tick_serial()
        # panels.render_panels(app)
        
    def post_init():
        imgui.dock_space_over_viewport(0, imgui.get_main_viewport(), imgui.DockNodeFlags_.passthru_central_node)
    # runner_params.callbacks.post_init_add_platform_backend_callbacks = post_init
    runner_params.callbacks.show_gui = show_gui
    immapp.run(runner_params)

if __name__ == "__main__":
    main()