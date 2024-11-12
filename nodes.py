import copy

# genome = {
#     "move_x": 0,
#     "move_y": 0,
#     "move_z": 0,
#     "split_dir": 0,   # 0: x, 1: y, 2: z
#     "right_child": None,
#     "left_child": None,
#     "neuron_id": 0
# }

possible_axis = [0, 1, 2]


def get_node(space, coords, axis, inc):
    ''' axis: {x:0, y: 1, z: 2} '''
    c = copy.deepcopy(coords)
    c[axis] += inc
    return space[c[0]][c[1]][c[2]]


def get_dimensions(space, coords, axis, space_size):
    pos_dir = 0
    neg_dir = 0
    c = copy.deepcopy(coords)
    for i in range(coords[axis]+1, space_size):
        c[axis] = i
        node = space[c[0]][c[1]][c[2]]
        if node is None:
            break
        # if node['blocked']:
        #     pos_dir = space_size
        #     break
        pos_dir += 1
    for i in range(coords[axis]-1, 0, -1):
        c[axis] = i
        node = space[c[0]][c[1]][c[2]]
        if node is None:
            break
        # if node['blocked']:
        #     neg_dir = space_size
        #     break
        neg_dir += 1
    return neg_dir, pos_dir


def divide_node(space, coords, space_size):
    node = space[coords[0]][coords[1]][coords[2]]
    if (node['left_child'] is None) and (node['right_child'] is None):
        return
    if (node['left_child'] is not None) and (node['right_child'] is None):
         space[coords[0]][coords[1]][coords[2]] = node['left_child']
         return
    if (node['left_child'] is None) and (node['right_child'] is not None):
         space[coords[0]][coords[1]][coords[2]] = node['right_child']
         return
    axis = node['split_dir']
    neg_dir, pos_dir = get_dimensions(space, coords, axis, space_size)
    
    c = copy.deepcopy(coords)
    if neg_dir < pos_dir:
        # Mode left node
        for i in range(coords[axis]-neg_dir, coords[axis]):
            c[axis] = i
            n = space[c[0]][c[1]][c[2]]
            c[axis] -= 1
            space[c[0]][c[1]][c[2]] = n
        c = copy.deepcopy(coords)
        c[axis] = coords[axis] - 1
        space[c[0]][c[1]][c[2]] = node['left_child']
        space[coords[0]][coords[1]][coords[2]] = node['right_child']
    else:
        if neg_dir == pos_dir == space_size:    # node stuck between blocked nodes
            node['split_dir'] = (axis + 1) % 3
            divide_node(space, coords, space_size)
            return
        # Mode right node
        for i in range(coords[axis]+pos_dir+1, coords[axis], -1):
            c[axis] = i-1
            n = space[c[0]][c[1]][c[2]]
            c[axis] = i
            space[c[0]][c[1]][c[2]] = n
        c = copy.deepcopy(coords)
        c[axis] = coords[axis] + 1
        space[c[0]][c[1]][c[2]] = node['right_child']
        space[coords[0]][coords[1]][coords[2]] = node['left_child']


def test():
    space_size = 5
    space = [[[None for i in range(space_size)] for j in range(space_size)] for k in range(space_size)]
    space[2][2][2] = {
        "move_x": 0,
        "move_y": 0,
        "move_z": 0,
        "split_dir": 0,   # 0: x, 1: y, 2: z
        "right_child": {
            "move_x": 0,
            "move_y": 0,
            "move_z": 0,
            "split_dir": 0,   # 0: x, 1: y, 2: z
            "right_child": {
                "move_x": 0,
                "move_y": 0,
                "move_z": 0,
                "split_dir": 0,   # 0: x, 1: y, 2: z
                "right_child": None,
                "left_child": None,
                "neuron_id": 0
            },
            "left_child": {
                "move_x": 0,
                "move_y": 0,
                "move_z": 0,
                "split_dir": 0,   # 0: x, 1: y, 2: z
                "right_child": None,
                "left_child": None,
                "neuron_id": 0
            },
            "neuron_id": 0
        },
        "left_child": {
            "move_x": 0,
            "move_y": 0,
            "move_z": 0,
            "split_dir": 0,   # 0: x, 1: y, 2: z
            "right_child": {
                "move_x": 0,
                "move_y": 0,
                "move_z": 0,
                "split_dir": 0,   # 0: x, 1: y, 2: z
                "right_child": None,
                "left_child": None,
                "neuron_id": 0
            },
            "left_child": {
                "move_x": 0,
                "move_y": 0,
                "move_z": 0,
                "split_dir": 0,   # 0: x, 1: y, 2: z
                "right_child": None,
                "left_child": None,
                "neuron_id": 0
            },
            "neuron_id": 0
        },
        "neuron_id": 0
    }
    divide_node(space, [2, 2, 2], space_size)
    divide_node(space, [2, 2, 2], space_size)
    divide_node(space, [3, 2, 2], space_size)
    for i in range(space_size):
        for j in range(space_size):
            for k in range(space_size):
                if space[i][j][k] is not None:
                    print(f"Node [{i}][{j}][{k}]")



if __name__ == "__main__":
    test()
