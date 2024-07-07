import subprocess
import os
import sys


files_to_build = ["main.c", "neuron.c", "network.c", "utils.c", "save_restore.c"]
output_file_name = "main"
flags = ["-lm", "-g", "-Wall"]


def main():
    os.remove(output_file_name) if os.path.exists(output_file_name) else None
    os.remove(f"{output_file_name}.exe") if os.path.exists(f"{output_file_name}.exe") else None
    print(subprocess.getoutput(f"gcc {' '.join(files_to_build)} {' '.join(flags)} -o {output_file_name}"))


if __name__ == "__main__":
    main()
