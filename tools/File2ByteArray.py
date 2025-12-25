import sys
import os

def file_to_c_array(filename, array_name="payloadBuffer"):
    try:
        with open(filename, "rb") as f:
            content = f.read()
    except FileNotFoundError:
        print(f"File not found: {filename}")
        sys.exit(1)

    hex_bytes = [f"0x{b:02x}" for b in content]
    array_str = f"unsigned char {array_name}[] = {{ " + ", ".join(hex_bytes) + " };"

    return array_str

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {os.path.basename(sys.argv[0])} <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    c_array = file_to_c_array(filename)
    print(c_array)
