import os
import sys
import importlib
import inspect
from types import ModuleType
from typing import List, Callable


class PanelManager:
    """
    Discovers panel modules in the given directory.
    Each panel_xxx.py file can define one or more functions named draw_*.
    Hot reload is supported: if a file changes, it is reloaded.
    """

    def __init__(self, panels_dir: str = "panels"):
        self.panels_dir = os.path.abspath(panels_dir)
        if self.panels_dir not in sys.path:
            sys.path.append(self.panels_dir)

        self.modules: dict[str, ModuleType] = {}
        self.mtimes: dict[str, float] = {}
        self.panel_funcs: List[Callable] = []

    def discover_panels(self):
        """Scan the panels directory, reload changed modules, collect draw_* functions."""
        funcs: List[Callable] = []
        if not os.path.isdir(self.panels_dir):
            return

        for fname in os.listdir(self.panels_dir):
            if not (fname.startswith("panel_") and fname.endswith(".py")):
                continue

            modname = fname[:-3]
            path = os.path.join(self.panels_dir, fname)

            try:
                mtime = os.path.getmtime(path)
            except OSError:
                continue

            try:
                # Reload if file changed
                if modname in self.modules and self.mtimes.get(modname) != mtime:
                    module = importlib.reload(self.modules[modname])
                    self.modules[modname] = module
                    self.mtimes[modname] = mtime
                    print(f"[PanelManager] Reloaded {modname}")
                elif modname not in self.modules:
                    module = importlib.import_module(modname)
                    self.modules[modname] = module
                    self.mtimes[modname] = mtime
                    print(f"[PanelManager] Loaded {modname}")
                else:
                    module = self.modules[modname]
            except Exception as e:
                print(f"[PanelManager] Error loading {modname}: {e}")
                continue

            # Collect draw_* functions
            for name, func in inspect.getmembers(module, inspect.isfunction):
                if name.startswith("draw_"):
                    funcs.append(func)

        self.panel_funcs = funcs

    def render_panels(self, app_state):
        """Call all discovered draw_* functions with the given app_state."""
        for func in self.panel_funcs:
            try:
                func(app_state)
            except Exception as e:
                print(f"[PanelManager] Error in {func.__name__}: {e}")
