import ctypes
from network import Network, create_network_map
from pearson import get_correlation, get_max_correlation
import time
import test_dataset


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
        temp_error = 0
        output = network.get_output(test_dataset[i][0])
        for j in range(len(test_dataset[i][1])):
            temp_error += (test_dataset[i][1][j] - output[j]) ** 2
        error += temp_error / len(test_dataset[i][1])
        # error += (test_dataset[i][1] - network.get_output(test_dataset[i][0])[0]) ** 2
    error /= len(test_dataset)
    return error


def print_error(network, test_dataset):
    # output = net.get_output(inputs)
    error = 0
    for i in range(len(test_dataset)):
        temp_error = 0
        output = network.get_output(test_dataset[i][0])
        # print(f"Inputs: {test_dataset[i][0]}")
        # print(f"Expected outputs: {test_dataset[i][1]}")
        # print(f"Real outputs    : {output}")
        for j in range(len(test_dataset[i][1])):
            temp_error += (test_dataset[i][1][j] - output[j]) ** 2
        e = temp_error / len(test_dataset[i][1])
        error += e
        print(f"Expected value: {test_dataset[i][1]}, got value: {output}, error: {e}")
    error /= len(test_dataset)
    print(f"Total error: {error}")
    # return error


def xor(a, b, c, d):
    output = a + b + c + d - (4 * a * b * c * d)
    return output


def test_correlations():
    arrs = [
        [0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1],
        [0.1, 0.1, 0.1, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1],
        [0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.3, 0.4, 0.3, 0.3, 0.3],
        [0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.2, 0.1, 0.2, 0.3, 0.3, 0.3],
        [-0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.3, -0.5, -0.3, -0.2, -0.2, -0.2],
        [-0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.3, -0.2, -0.1, -0.2, -0.3, -0.4, -0.4]]

    # dataset = []
    # for i in arrs[1:]:
    #     dataset.append([arrs[0]+i, get_max_correlation(arrs[0], i)])
    
    # print(dataset)
    dataset = [
        [[0.0, 0.0, 0.0, 0.0], [0.0]],
        [[0.0, 0.0, 0.0, 1.0], [1.0]],
        [[0.0, 0.0, 1.0, 0.0], [1.0]],
        [[0.0, 0.0, 1.0, 1.0], [1.0]],
        [[0.0, 1.0, 0.0, 0.0], [1.0]],
        [[0.0, 1.0, 0.0, 1.0], [1.0]],
        [[0.0, 1.0, 1.0, 0.0], [1.0]],
        [[0.0, 1.0, 1.0, 1.0], [1.0]],
        [[1.0, 0.0, 0.0, 0.0], [1.0]],
        [[1.0, 0.0, 0.0, 1.0], [1.0]],
        [[1.0, 0.0, 1.0, 0.0], [1.0]],
        [[1.0, 0.0, 1.0, 1.0], [1.0]],
        [[1.0, 1.0, 0.0, 0.0], [1.0]],
        [[1.0, 1.0, 0.0, 1.0], [1.0]],
        [[1.0, 1.0, 1.0, 0.0], [1.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
        [[1.0, 1.0, 1.0, 1.0], [0.0]],
    ]
    # dataset = []
    # for i in range(256):
    #     dataset.append(test_dataset.get_example(i))

    # for i in dataset:
    #     print(f"{i[0]}: {xor(i[0][0], i[0][1], i[0][2], i[0][3])}")
    # return

    net_map = create_network_map([4, 4, 1], -1)
    for i in net_map:
        print(i)
    net = Network("bin/controller.dll", net_map, 5)
    
    print_error(net, dataset)
    prev_error = get_error(net, dataset)
    print(f"Init error: {prev_error}")
    # net.print_coeffs()
    min_error = 0.1
    counter = 0
    while (prev_error > min_error) and (counter < 10000):
        net.mutate()
        new_error = get_error(net, dataset)
        if new_error > prev_error:
            net.restore()
        elif new_error < prev_error:
            print(f"New error: {new_error}")
            prev_error = new_error
        time.sleep(0.01)
        counter += 1
    print_error(net, dataset)
    # net.print_coeffs()
    
    # net.save_state()


if __name__ == '__main__':
    test_correlations()
