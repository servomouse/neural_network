import ctypes
from ctypes_wrapper import get_dll_function
import random


class Nanite:
    def __init__(self, filename, num_inputs):
        print(f"Creating neuron with {num_inputs} inputs")
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
        self.init(num_inputs)
        # print("Nanite initialized!")


class Network:
    def __init__(self, filename, net_map:list, network_depth):
        """ network_depth: how many times to updat ethe network before getting the result """
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
        self.last_mutated_neuron = 0
        for i in range(self.num_neurons):
            if net_map[i]['type'] == 'input':
                self.num_inputs += 1
            elif net_map[i]['type'] == 'output':
                self.num_outputs += 1
        print(f"num_inputs = {self.num_inputs}, num_neurons = {self.num_neurons}, num_outputs = {self.num_outputs}")
        self.init(self.num_inputs, self.num_neurons)
        for i in range(self.num_neurons):
            if net_map[i]['type'] == 'input':
                self.neurons.append(None)
            else:
                nanite = Nanite("bin/nanite.dll", len(net_map[i]['inputs']))
                self.set_output_function(i-self.num_inputs, nanite.get_output_p)
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
    
    def mutate(self):
        self.last_mutated_neuron = random.randint(self.num_inputs, self.num_neurons-1)
        try:
            self.neurons[self.last_mutated_neuron].mutate()
        except IndexError:
            print(f"Tried to mutate neuron {self.last_mutated_neuron}!")
            raise Exception("AAAAAAAAAA!!!!!1")
    
    def restore(self):
        self.neurons[self.last_mutated_neuron].restore()



def create_network_map(layers: list, max_neuron_inputs_num: int):
    """ Set max_neuron_inputs_num to -1 to disable limit of numbers of inputs """
    network = []
    counter = 0
    num_neurons = 0
    for i in layers:
        num_neurons += i
    for layer in layers:
        network.append([])
        for i in range(layer):
            network[-1].append(counter)
            counter += 1
    links = {}
    for i in range(1, len(network)):
        for j in network[i]:
            if max_neuron_inputs_num == -1:
                links[f"{j}"] = network[i-1]
            else:
                if len(network[i-1]) > (len(network[i]) * max_neuron_inputs_num):
                    print(f"Warning: layer {i-1} is too wide or layer {i} is too small: cannot connect all the neurons from layer {i-1} to layer {i}!")
                if len(network[i-1]) > max_neuron_inputs_num:
                    links[f"{j}"] = random.sample(network[i-1], max_neuron_inputs_num)
                else:
                    links[f"{j}"] = network[i-1]
    map = []
    for i in range(layers[0]):
        map.append({"type": "input", "inputs": [], "id": i})
    for key, value in links.items():
        map.append({"type": "hidden", "inputs": value, "id": int(key)})
    num_outputs = layers[-1]
    for i in range(num_neurons - num_outputs, num_neurons):
        map[i]["type"] = "output"
    return map
