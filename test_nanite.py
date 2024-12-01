import ctypes
from ctypes_wrapper import get_dll_function


class Nanite:
    def __init__(self, filename):
        self.nanite = ctypes.CDLL(filename)
        self.init = get_dll_function(self.nanite, "void init(uint32_t)")
        self.set_input_idx = get_dll_function(self.nanite, "void set_input_idx(uint32_t, uint32_t)")
        self.save_state = get_dll_function(self.nanite, "void save_state(char*)")
        self.restore_state = get_dll_function(self.nanite, "void restore_state(char*)")
        self.get_output = get_dll_function(self.nanite, "double get_output(double*)")
        self.init(8)
        print("Nanite initialized!")
    
    def test_nanite(self):
        DoubleArrayType = ctypes.c_double * 8
        double_array = DoubleArrayType(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1)
        for i in range(8):
            self.set_input_idx(i, i)
        init_output = self.get_output(double_array)
        print(f"Init output: {init_output}")
        self.save_state(b"test_nanite.bin")
        self.init(8)
        temp_output = self.get_output(double_array)
        print(f"Temp output: {temp_output}")
        self.restore_state(b"test_nanite.bin")
        new_output = self.get_output(double_array)
        print(f"New output: {new_output}")


def main():
    filename = "bin/nanite.dll"
    nanite = Nanite(filename)
    nanite.test_nanite()


if __name__ == "__main__":
    main()