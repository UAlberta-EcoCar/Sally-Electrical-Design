from imgui_bundle import imgui, immapp, glfw_utils
import OpenGL.GL as gl
import requests
from PIL import Image
from io import BytesIO
import math


# -----------------------------
# TILE FETCHING + TEXTURE CACHE
# -----------------------------
TILE_CACHE = {}

def fetch_tile(z, x, y):
    """Fetch OSM tile and upload as OpenGL texture."""
    key = (z, x, y)
    if key in TILE_CACHE:
        return TILE_CACHE[key]

    url = f"https://tile.openstreetmap.org/{z}/{x}/{y}.png"
    r = requests.get(url)
    if r.status_code != 200:
        return None

    img = Image.open(BytesIO(r.content)).convert("RGBA")
    tex_id = upload_texture(img)
    TILE_CACHE[key] = tex_id
    return tex_id


def upload_texture(image):
    """Upload PIL image to OpenGL texture."""
    w, h = image.size
    data = image.tobytes()

    tex_id = gl.glGenTextures(1)
    gl.glBindTexture(gl.GL_TEXTURE_2D, tex_id)
    gl.glTexImage2D(gl.GL_TEXTURE_2D, 0, gl.GL_RGBA, w, h, 0,
                    gl.GL_RGBA, gl.GL_UNSIGNED_BYTE, data)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER, gl.GL_LINEAR)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MAG_FILTER, gl.GL_LINEAR)
    return tex_id


# -----------------------------
# MAIN MAP VIEWER
# -----------------------------
zoom = 4
offset_x = 0.0
offset_y = 0.0
dragging = False
last_mouse = (0, 0)


def gui():
    global zoom, offset_x, offset_y, dragging, last_mouse

    imgui.begin("OpenStreetMap Viewer")

    # Window geometry
    win_x, win_y = imgui.get_window_position()
    win_w, win_h = imgui.get_window_size()
    draw_list = imgui.get_window_draw_list()

    # Mouse input
    io = imgui.get_io()
    mx, my = io.mouse_pos
    left_down = io.mouse_down[0]

    # Start drag
    if left_down and not dragging:
        dragging = True
        last_mouse = (mx, my)

    # Dragging
    if dragging and left_down:
        dx = mx - last_mouse[0]
        dy = my - last_mouse[1]
        offset_x += dx
        offset_y += dy
        last_mouse = (mx, my)

    # Stop drag
    if not left_down:
        dragging = False

    # Tile size
    TILE = 256
    tiles_per_edge = 2 ** zoom

    # Convert pixel offset to tile offset
    tile_offset_x = offset_x / TILE
    tile_offset_y = offset_y / TILE

    # Center tile index
    center_tile_x = tiles_per_edge / 2 - tile_offset_x
    center_tile_y = tiles_per_edge / 2 - tile_offset_y

    # Draw tiles around center
    for dx in range(-4, 5):
        for dy in range(-4, 5):
            tx = int(center_tile_x + dx)
            ty = int(center_tile_y + dy)

            if 0 <= tx < tiles_per_edge and 0 <= ty < tiles_per_edge:
                tex = fetch_tile(zoom, tx, ty)
                if tex:
                    px = win_x + win_w/2 + (tx - center_tile_x) * TILE
                    py = win_y + win_h/2 + (ty - center_tile_y) * TILE
                    draw_list.add_image(tex, px, py, px + TILE, py + TILE)

    imgui.end()


# -----------------------------
# RUN APP
# -----------------------------
if __name__ == "__main__":
    immapp.run(gui, window_title="OSM Map Viewer (imgui-bundle)")
