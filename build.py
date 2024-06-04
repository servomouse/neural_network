import subprocess


files_to_build = ["main.c", "neuron.c", "network.c"]
output_file_name = "main"
flags = ["-lm"]


def main():
    print(subprocess.getoutput(f"gcc {' '.join(files_to_build)} {' '.join(flags)} -o {output_file_name}"))


if __name__ == "__main__":
    main()
