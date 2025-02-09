import os
import subprocess
import sys

# build_files = ["nanite.c", "utils.c"]
common_flags = ["-lm", "-g", "-Wall", "-lws2_32", "-I.", "-Ibackups", "-fstack-protector-all"]

targets = {
    "neuron_linear": {
        "build_flags": common_flags + ["-shared"],
        "build_files": ["neuron_linear.c", "utils.c"],
        "output_file": "bin/neuron_linear.dll"
    },
    "neuron_smart": {
        "build_flags": common_flags + ["-shared"],
        "build_files": ["neuron_smart.c", "utils.c"],
        "output_file": "bin/neuron_smart.dll"
    },
    "neuron_test": {
        "build_flags": common_flags,
        "build_files": ["test/neuron_test.c", "neuron_smart.c", "utils.c", "neuron_complex_values.c"],
        "output_file": "bin/neuron_test.exe"
    },
    "controller": {
        "build_flags": common_flags + ["-shared"],
        "build_files": ["neuron_kapellmeister.c", "neuron_smart.c", "micro_net.c", "utils.c"],
        "output_file": "bin/controller.dll"
    },
    "micronet_test": {
        "build_flags": common_flags,
        "build_files": ["tests/micronet_test.c", "neuron_simple.c", "micro_net.c", "utils.c"],
        "output_file": "bin/micronet_test.exe"
    },
    "network": {
        "build_flags": common_flags + ["-shared"],
        "build_files": ["network_new.c", "neuron_smart.c", "micro_net.c", "utils.c"],
        "output_file": "bin/network.dll"
    },
    "network_test": {
        "build_flags": common_flags,
        "build_files": ["tests/network_test.c", "neuron_smart.c", "neuron_simple.c", "network_new.c", "micro_net.c", "utils.c", "neuron_complex_values.c"],
        "output_file": "bin/network_test.exe"
    },
}


def main(target):
    if target not in targets:
        print(f"Error: invalid target: {target}!")
        return
    fname = targets[target]["output_file"]
    if os.path.isfile(fname):
        os.remove(fname)
    srcs = ' '.join(targets[target]["build_files"])
    flags = ' '.join(targets[target]["build_flags"])
    print(f"Building {target} . . . ", end='', flush=True)
    cmd = f"gcc {flags} {srcs} -o {fname}"
    output = subprocess.getoutput(cmd)
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
