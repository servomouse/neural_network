import ctypes
from network import Network


neuron_ids = {
    4: {
        'inputs': [[0, 0, 0], [0, 0, 1], [0, 0, 2], [0, 0, 3]],    # Coordinates relative to the neuron position
        'weights': [0.1, 0.1, 0.1, 0.1, 0.1]
    }
}


test_net_map = [
    {
        "type": "input",
        "inputs": [],       # Input neurons don't have inputs
        "id": 0,            # This field is ignored for input neurons
    },
    {
        "type": "input",
        "inputs": [],       # Input neurons don't have inputs
        "id": 0,            # This field is ignored for input neurons
    },
    {
        "type": "input",
        "inputs": [],       # Input neurons don't have inputs
        "id": 0,            # This field is ignored for input neurons
    },
    {
        "type": "input",
        "inputs": [],       # Input neurons don't have inputs
        "id": 0,            # This field is ignored for input neurons
    },
    # {
    #     "type": "regular",
    #     "inputs": [0, 1, 2],    # Indices in the array
    #     "id": 4,
    # },
    {
        "type": "output",
        "inputs": [0, 1, 2, 4],    # Indices in the array
        "id": 4,
    },
]

def main():
    net = Network("bin/controller.dll", test_net_map, 1)
    inputs = [0.1, 0.1, 0.1, 0.1]
    output = net.get_output(inputs)
    print(f"Output = {output[0]}")


if __name__ == '__main__':
    main()
