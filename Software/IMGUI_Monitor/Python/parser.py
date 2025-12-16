import re

def parse_line(line: str):
    """
    Parse a UART line into a dict of key=value pairs.
    Expected format: "V=12.4,I=35.1,RLY1=ON,RLY2=OFF"
    Returns (status, dict).
    """
    data = {}
    try:
        for part in line.split(","):
            if "=" in part:
                k, v = part.split("=", 1)
                data[k.strip()] = v.strip()
        # Try to cast numeric values
        for k, v in list(data.items()):
            if re.match(r"^[+-]?\d+(\.\d+)?$", v):
                try:
                    data[k] = float(v)
                except ValueError:
                    pass
        return ("ok", data)
    except Exception as e:
        return ("error", {"raw": line, "error": str(e)})
