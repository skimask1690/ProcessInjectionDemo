import sys

def file_to_c_array(filename, array_name="payloadBuffer"):
    with open(filename, "rb") as f:
        content = f.read()

    hex_bytes = [f"0x{b:02x}" for b in content]

    lines = []
    line_length = 12  # bytes per line
    for i in range(0, len(hex_bytes), line_length):
        lines.append(", ".join(hex_bytes[i:i+line_length]))

    array_str = f"unsigned char {array_name}[] = {{\n    " + ",\n    ".join(lines) + "\n};\n"

    return array_str

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: python {sys.argv[0]} <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    c_array = file_to_c_array(filename)
    print(c_array)
