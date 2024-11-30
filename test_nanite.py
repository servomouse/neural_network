import ctypes
import os


def get_type(item):
    if item == "void":
        return None
    if item == "int":
        return ctypes.c_int
    if item == "int*":
        return ctypes.POINTER(ctypes.c_int)
    if item == "char":
        return ctypes.c_char
    if item == "char*":
        return ctypes.c_char_p
    if item == "uint8_t":
        return ctypes.c_uint8
    if item == "uint8_t*":
        return ctypes.POINTER(ctypes.c_uint8)
    if item == "uint16_t":
        return ctypes.c_uint16
    if item == "uint16_t*":
        return ctypes.POINTER(ctypes.c_uint16)
    if item == "uint32_t":
        return ctypes.c_uint32
    if item == "uint32_t*":
        return ctypes.POINTER(ctypes.c_uint32)
    if item == "double":
        return ctypes.c_double
    if item == "double*":
        return ctypes.POINTER(ctypes.c_double)
    print(f"ERROR: Unknown type: {item}")
    os.exit(1)


# name = b'Test Name'
# example.set_name(name)
# print("Name: ", example.get_name().decode('utf-8'))
# value = ctypes.c_double(42.0)
# example.set_value(ctypes.byref(value))
# print("Value: ", example.get_value())
# DoubleArrayType = ctypes.c_double * 5
# double_array = DoubleArrayType(1.1, 2.2, 3.3, 4.4, 5.5)
# example.set_value(double_array)
# result_ptr = example.get_value()
# result_arr = [result_ptr[i] for i in range(5)]


def get_dll_function(dll_object, signature: str):
    ''' Signature looks like this: "void foo (int, uint8_t)"
        where "foo" is the actual name of the function '''
    remove_symbols = [',', '(', ')']
    for s in remove_symbols:
        signature = signature.replace(s, ' ')
    items = signature.split()
    res_type = get_type(items[0])
    params = []
    for i in items[2:]:
        params.append(get_type(i))
    foo = getattr(dll_object, items[1])
    if params[0] is None:
        foo.argtypes = None
    else:
        foo.argtypes = params
    foo.restype = res_type
    return foo


class Nanite:
    def __init__(self, filename):
        self.nanite = ctypes.CDLL(filename)
        self.init = get_dll_function(self.nanite, "void init(uint32_t)")
        self.set_input_idx = get_dll_function(self.nanite, "void set_input_idx(uint32_t, uint32_t)")
        self.save_state = get_dll_function(self.nanite, "void save_state(char*)")
        self.restore_state = get_dll_function(self.nanite, "void restore_state(char*)")
        self.get_output = get_dll_function(self.nanite, "double get_output(double*)")


def main():
    filename = "bin/nanite_dll"
    nanite = ctypes.CDLL(filename)
    self.set_log_level = get_dll_function(nanite, "void set_log_level(uint8_t)")