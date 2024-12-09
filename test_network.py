import ctypes
from network import Network, create_network_map
from pearson import get_correlation, get_max_correlation
import time


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


def round_num(value):
    return (int(value+100)/100)


def simple_test():
    net = Network("bin/controller.dll", test_net_map, 1)
    inputs = [0.1, 0.1, 0.1, 0.1]
    output = net.get_output(inputs)
    print(f"Output = {output[0]}")


def get_error(network, test_dataset):
    # output = net.get_output(inputs)
    error = 0
    for i in range(len(test_dataset)):
        error += (test_dataset[i][1] - network.get_output(test_dataset[i][0])[0]) ** 2
    error /= len(test_dataset)
    return error


def print_error(network, test_dataset):
    # output = net.get_output(inputs)
    error = 0
    for i in range(len(test_dataset)):
        val = network.get_output(test_dataset[i][0])[0]
        error += (test_dataset[i][1] - val) ** 2
        print(f"Expected value: {test_dataset[i][1]}, got value: {val}, error: {error}")
    error /= len(test_dataset)
    print(f"Total error: {error}")
    # return error


def test_correlations():
    arrs = [
        [0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1],
        [0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1],
        [0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.3, 0.4, 0.3, 0.3, 0.3],
        [0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.2, 0.1, 0.2, 0.3, 0.3, 0.3],
        [-0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.3, -0.5, -0.3, -0.2, -0.2, -0.2],
        [-0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.3, -0.2, -0.1, -0.2, -0.3, -0.4, -0.4]]

    dataset = []
    for i in arrs[1:]:
        dataset.append([arrs[0]+i, get_max_correlation(arrs[0], i)])
    
    print(dataset)

    net_map = create_network_map([40, 80, 40, 20, 10, 1], 8)
    for i in net_map:
        print(i)
    net = Network("bin/controller.dll", net_map, 5)
    
    prev_error = get_error(net, dataset)
    print(f"Init error: {prev_error}")
    min_error = 0.055
    counter = 0
    while (prev_error > min_error) and (counter < 1000):
        net.mutate()
        new_error = get_error(net, dataset)
        if new_error > prev_error:
            net.restore()
        else:
            print(f"New error: {new_error}")
            prev_error = new_error
        time.sleep(0.01)
        counter += 1
    print_error(net, dataset)
    
    # net.save_state()

    # for i in arrs[1:]:
    #     c = get_max_correlation(arrs[0], i)
    #     # print(i, ", max_correlation: ", get_max_correlation(arrs[0], i))
    #     inputs = arrs[0] + i
    #     print(inputs)
    #     output = net.get_output(inputs)
    #     print(f"Output = {output[0]}")


if __name__ == '__main__':
    test_correlations()
