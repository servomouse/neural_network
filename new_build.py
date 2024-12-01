import os
import subprocess
import sys

build_files = ["nanite.c", "utils.c"]
common_flags = ["-lm", "-g", "-Wall", "-lws2_32"]

targets = {
    "nanite": {
        "build_files": ["nanite.c", "utils.c"],
        "output_file": "nanite.dll"
    }
}


def main(target):
    print(f"Building {target} . . . ", end='', flush=True)
    fname = f"bin/{target}.dll"
    if os.path.isfile(fname):
        os.remove(fname)
    srcs = ' '.join(targets[target]["build_files"])
    flags = ' '.join(common_flags)
    output = subprocess.getoutput(f"gcc -shared {flags} {srcs} -o {fname}")
    if len(output) == 0:
        print("Done")
        return 0
    else:
        print(f"Failed\nERROR: {output}")
        return 1


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("Usage: python new_build.py target_name")
    else:
        main(sys.argv[1])
