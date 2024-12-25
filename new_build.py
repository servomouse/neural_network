import os
import subprocess
import sys

# build_files = ["nanite.c", "utils.c"]
common_flags = ["-lm", "-g", "-Wall", "-lws2_32"]

targets = {
    "neuron_linear": {
        "build_files": ["neuron_linear.c", "utils.c"],
        "output_file": "neuron_linear.dll"
    },
    "neuron_smart": {
        "build_files": ["neuron_smart.c", "utils.c"],
        "output_file": "neuron_smart.dll"
    },
    "controller": {
        "build_files": ["neuron_kapellmeister.c", "neuron_smart.c", "utils.c"],
        "output_file": "controller.dll"
    },
}


def main(target):
    if target not in targets:
        print(f"Error: invalid target: {target}!")
        return
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
        for target in targets:
            main(target)
    else:
        if sys.argv[1] == 'help':
            print(f"Available targets: {[i for i in targets]}")
        else:
            main(sys.argv[1])
