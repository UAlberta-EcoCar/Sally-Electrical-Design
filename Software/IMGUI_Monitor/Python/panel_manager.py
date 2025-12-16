import os, sys, importlib, inspect, time

class PanelManager:
    def __init__(self, panels_dir="panels"):
        self.panels_dir = os.path.join(os.path.dirname(__file__), panels_dir)
        sys.path.append(self.panels_dir)
        self.modules = {}
        self.mtimes = {}
        self.panel_funcs = []

    def discover_panels(self):
        funcs = []
        for fname in os.listdir(self.panels_dir):
            if fname.startswith("panel_") and fname.endswith(".py"):
                modname = fname[:-3]
                path = os.path.join(self.panels_dir, fname)
                mtime = os.path.getmtime(path)

                # Reload if new or changed
                if modname not in self.modules or self.mtimes.get(modname) != mtime:
                    try:
                        if modname in self.modules:
                            module = importlib.reload(self.modules[modname])
                        else:
                            module = importlib.import_module(modname)
                        self.modules[modname] = module
                        self.mtimes[modname] = mtime
                        print(f"[PanelManager] Reloaded {modname}")
                    except Exception as e:
                        print(f"[PanelManager] Error loading {modname}: {e}")
                        continue

                # Collect draw_* functions
                module = self.modules[modname]
                for name, func in inspect.getmembers(module, inspect.isfunction):
                    if name.startswith("draw_"):
                        funcs.append(func)
        self.panel_funcs = funcs

    def render_panels(self, app):
        for func in self.panel_funcs:
            try:
                func(app)
            except Exception as e:
                print(f"[PanelManager] Error in {func.__name__}: {e}")
