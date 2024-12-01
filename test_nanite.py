import ctypes
from ctypes_wrapper import get_dll_function
from network import Nanite


def test_nanite(nanite):
    DoubleArrayType = ctypes.c_double * 8
    double_array = DoubleArrayType(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1)
    for i in range(8):
        nanite.set_input_idx(i, i)
    init_output = nanite.get_output(double_array)
    print(f"Init output: {init_output}")
    nanite.save_state(b"test_nanite.bin")
    nanite.init(8)
    temp_output = nanite.get_output(double_array)
    print(f"Temp output: {temp_output}")
    nanite.restore_state(b"test_nanite.bin")
    new_output = nanite.get_output(double_array)
    print(f"New output: {new_output}")


def main():
    filename = "bin/nanite.dll"
    nanite = Nanite(filename)
    test_nanite(nanite)


if __name__ == "__main__":
    main()