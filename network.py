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
        self.mutate = get_dll_function(self.nanite, "void mutate(void)")
        self.restore = get_dll_function(self.nanite, "void restore(void)")
        get_output_func_p = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.POINTER(ctypes.c_double))
        self.get_output_p = get_output_func_p(self.get_output)
        self.init(8)
        print("Nanite initialized!")


class Network:
    def __init__(self, filename, net_map:list, network_depth):
        self.network_depth = network_depth
        self.controller = ctypes.CDLL(filename)
        self.init = get_dll_function(self.controller, "void init(uint32_t, uint32_t)")
        
        get_output_func_t = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.POINTER(ctypes.c_double))
        self.controller.set_output_function.argtypes = [ctypes.c_uint32, get_output_func_t]
        self.controller.set_output_function.restype = None
        self.set_output_function = self.controller.set_output_function

        self.set_value = get_dll_function(self.controller, "void set_value(uint32_t, double)")
        self.get_value = get_dll_function(self.controller, "double get_value(uint32_t)")
        self.tick_network = get_dll_function(self.controller, "void tick_network(void)")
        self.swap_arrays = get_dll_function(self.controller, "void swap_arrays(void)")
        self.save_state = get_dll_function(self.controller, "void save_state(char*)")
        self.restore_state = get_dll_function(self.controller, "void restore_state(char*)")
        self.num_neurons = len(net_map)
        self.num_inputs = 0
        self.num_outputs = 0
        self.neurons = []
        for i in range(self.num_neurons):
            if net_map[i]['type'] == 'input':
                self.num_inputs += 1
            elif net_map[i]['type'] == 'output':
                self.num_outputs += 1
        print(f"num_inputs = {self.num_inputs}, num_neurons = {self.num_neurons}, num_outputs = {self.num_outputs}")
        self.init(self.num_inputs, self.num_neurons-self.num_inputs)
        for i in range(self.num_neurons):
            if net_map[i]['type'] == 'input':
                self.neurons.append(None)
            else:
                nanite = Nanite("bin/nanite.dll")
                self.set_output_function(i, nanite.get_output_p)
                for j in range(len(net_map[i]['inputs'])):
                    nanite.set_input_idx(j, net_map[i]['inputs'][j])
                self.neurons.append(nanite)

    def get_output(self, inputs):
        if len(inputs) != self.num_inputs:
            print("Error: incorrect inputs len({len(inputs)})!")
            return 0
        for i in range(self.num_inputs):
            self.set_value(i, inputs[i])
        for i in range(self.network_depth):
            self.tick_network()
            self.swap_arrays()
        ret_vals = []
        for i in range(self.num_neurons - self.num_outputs, self.num_neurons):
            ret_vals.append(self.get_value(i))
        return ret_vals
