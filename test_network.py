import ctypes
from ctypes_wrapper import get_dll_function


neuron_ids = {
    0: {
        'inputs': [[0, 1, 2], [0, 2, 2], [0, 3, 2]],    # Coordinates relative to the neuron position
        'weights': [0.1, 0.1, 0.1, 0.1]
    }
}


net_map = [
    {
        "type": "input",
        "inputs": [],       # Input neurons don't have inputs
        "id": 0,            # This field is ignored for input neurons
    },
    {
        "type": "regular",
        "inputs": [0, 1, 2],    # Indices in the array
        "id": 5,
    },
    {
        "type": "output",
        "inputs": [5, 6, 7],    # Indices in the array
        "id": 8,
    }
]


class Network:
    def __init__(self, net_map:dict):
        pass
