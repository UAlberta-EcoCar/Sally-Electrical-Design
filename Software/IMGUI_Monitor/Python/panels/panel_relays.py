from imgui_bundle import imgui
from imgui_bundle import nanovg

def draw_process_flow(app_state):
    """
    relay_states: dict {relay_id: True/False}
    current_flows: list of (src, dst, flowing)
    """

    # Start an ImGui window
    imgui.begin("Process Flow")

    

    imgui.end()
