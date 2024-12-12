import ctypes
import os
import ctypes.wintypes


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


def free_library(handle):
    """
    https://stackoverflow.com/a/62326340
    Usage:
    import ctypes
    import ctypes.wintypes  # Needs to be explicitly imported
    lib = ctypes.CDLL("foobar.dll")
    free_library(lib._handle)
    """

    kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel32.FreeLibrary.argtypes = [ctypes.wintypes.HMODULE]
    kernel32.FreeLibrary(handle)


# ['ARRAY', 'ArgumentError', 'Array', 'BigEndianStructure', 'CDLL', 'CFUNCTYPE', 'DEFAULT_MODE', 'DllCanUnloadNow', 'DllGetClassObject', 'FormatError', 'GetLastError', 'HRESULT', 'LibraryLoader', 'LittleEndianStructure', 'OleDLL', 'POINTER', 'PYFUNCTYPE', 'PyDLL', 'RTLD_GLOBAL', 'RTLD_LOCAL', 'SetPointerType', 'Structure', 'Union', 'WINFUNCTYPE', 'WinDLL', 'WinError', '_CFuncPtr', '_FUNCFLAG_CDECL', '_FUNCFLAG_PYTHONAPI', '_FUNCFLAG_STDCALL', '_FUNCFLAG_USE_ERRNO', '_FUNCFLAG_USE_LASTERROR', '_Pointer', '_SimpleCData', '__builtins__', '__cached__', '__doc__', '__file__', '__loader__', '__name__', '__package__', '__path__', '__spec__', '__version__', '_c_functype_cache', '_calcsize', '_cast', '_cast_addr', '_check_HRESULT', '_check_size', '_ctypes_version', '_dlopen', '_endian', '_memmove_addr', '_memset_addr', '_os', '_pointer_type_cache', '_reset_cache', '_string_at', '_string_at_addr', '_sys', '_win_functype_cache', '_wstring_at', '_wstring_at_addr', 'addressof', 'alignment', 'byref', 'c_bool', 'c_buffer', 'c_byte', 'c_char', 'c_char_p', 'c_double', 'c_float', 'c_int', 'c_int16', 'c_int32', 'c_int64', 'c_int8', 'c_long', 'c_longdouble', 'c_longlong', 'c_short', 'c_size_t', 'c_ssize_t', 'c_ubyte', 'c_uint', 'c_uint16', 'c_uint32', 'c_uint64', 'c_uint8', 'c_ulong', 'c_ulonglong', 'c_ushort', 'c_void_p', 'c_voidp', 'c_wchar', 'c_wchar_p', 'cast', 'cdll', 'create_string_buffer', 'create_unicode_buffer', 'get_errno', 'get_last_error', 'memmove', 'memset', 'oledll', 'pointer', 'py_object', 'pydll', 'pythonapi', 'resize', 'set_errno', 'set_last_error', 'sizeof', 'string_at', 'windll', 'wstring_at']
