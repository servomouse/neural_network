import ctypes
from ctypes_wrapper import get_dll_function
from network import Nanite
import time


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


def get_error(nanite, test_dataset):
    error = 0
    for i in range(len(test_dataset)):
        error += (test_dataset[i][1] - nanite.get_output(test_dataset[i][0])) ** 2
    error /= len(test_dataset)
    return error


def test_evolution(nanite):
    DoubleArrayType = ctypes.c_double * 8
    test_dataset = [
        [DoubleArrayType(0.9, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1), 0.1],
        [DoubleArrayType(0.1, 0.9, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1), 0.2],
        [DoubleArrayType(0.1, 0.1, 0.9, 0.1, 0.1, 0.1, 0.1, 0.1), 0.3],
        [DoubleArrayType(0.1, 0.1, 0.1, 0.9, 0.1, 0.1, 0.1, 0.1), 0.4],
        [DoubleArrayType(0.1, 0.1, 0.1, 0.1, 0.9, 0.1, 0.1, 0.1), 0.5],
        [DoubleArrayType(0.1, 0.1, 0.1, 0.1, 0.1, 0.9, 0.1, 0.1), 0.6],
        [DoubleArrayType(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.9, 0.1), 0.7],
        [DoubleArrayType(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.9), 0.8],
    ]
    for i in range(8):
        nanite.set_input_idx(i, i)
    
    prev_error = get_error(nanite, test_dataset)
    print(f"Init error: {prev_error}")
    min_error = 0.055
    counter = 0
    while (prev_error > min_error) and (counter < 1000):
        nanite.mutate()
        new_error = get_error(nanite, test_dataset)
        if new_error > prev_error:
            nanite.restore()
        else:
            print(f"New error: {new_error}")
            prev_error = new_error
        time.sleep(0.1)
        counter += 1
    print(f"Evolved in {counter} steps!")
    nanite.save_state(b"test_nanite.bin")
    nanite.init(8)
    temp_error = get_error(nanite, test_dataset)
    print(f"Temp error: {temp_error}")
    nanite.restore_state(b"test_nanite.bin")
    new_error = get_error(nanite, test_dataset)
    print(f"New error: {new_error}")


def main():
    filename = "bin/nanite.dll"
    nanite = Nanite(filename)
    test_nanite(nanite)
    filename = "bin/nanite.dll"
    nanite = Nanite(filename)
    test_evolution(nanite)


if __name__ == "__main__":
    main()