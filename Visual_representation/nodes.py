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


def count_nodes(space, space_size):
    node_count = 0
    for i in range(space_size):
        for j in range(space_size):
            for k in range(space_size):
                if space[i][j][k] is not None:
                    node_count += 1
    return node_count


def get_node(space, coords, axis, inc):
    ''' axis: {x:0, y: 1, z: 2} '''
    c = copy.deepcopy(coords)
    c[axis] += inc
    return space[c[0]][c[1]][c[2]]


def get_dimensions(space, coords, axis, space_size):
    pos_dir = 0
    neg_dir = 0
    c = copy.deepcopy(coords)
    cell_found = False
    for i in range(coords[axis]+1, space_size):
        c[axis] = i
        node = space[c[0]][c[1]][c[2]]
        pos_dir += 1
        if node is None:
            cell_found = True
            break
        # if node['blocked']:
        #     pos_dir = space_size
        #     break
    if not cell_found:
        pos_dir = space_size
    cell_found = False
    for i in range(coords[axis]-1, 0, -1):
        c[axis] = i
        node = space[c[0]][c[1]][c[2]]
        neg_dir += 1
        if node is None:
            cell_found = True
            break
        # if node['blocked']:
        #     neg_dir = space_size
        #     break
    if not cell_found:
        neg_dir = space_size
    return neg_dir, pos_dir


def copy_coords(coords):
    return [coords[0], coords[1], coords[2]]


def divide_node(space, coords, space_size, step):
    ''' Returns coordinates of childs '''
    coords = copy.deepcopy(coords)
    node = space[coords[0]][coords[1]][coords[2]]
    if (node['left_child'] is None) and (node['right_child'] is None):
        return [coords]
    if (node['left_child'] is not None) and (node['right_child'] is None):
        space[coords[0]][coords[1]][coords[2]] = node['left_child']
        return [coords]
    if (node['left_child'] is None) and (node['right_child'] is not None):
        space[coords[0]][coords[1]][coords[2]] = node['right_child']
        return [coords]
    axis = node['split_dir']
    neg_dir, pos_dir = get_dimensions(space, coords, axis, space_size)
    
    c = copy.deepcopy(coords)
    
    if (neg_dir == pos_dir == space_size) \
        or ((neg_dir >= coords[axis]) and ((coords[axis] + pos_dir) >= space_size)):    # the row is full
        if c[axis] < space_size - 1:
            c[axis] += 1
        else:
            c[axis] -= 1
        if step == 5:
            raise Exception(f"Cannot divide node at {coords = }, {space[c[0]][c[1]][c[2]] = }")
        if step == 4:
            move_node(space, c, [0, 0, 0], space_size, True)
            return divide_node(space, coords, space_size, 4)
        else:
            node['split_dir'] = (axis + 1) % 3
            return divide_node(space, coords, space_size, step + 1)

    nodes_num = count_nodes(space, space_size)
    if neg_dir < pos_dir:
        # Mode left node
        if neg_dir > 1:
            for i in range(coords[axis]-neg_dir, coords[axis]-1):
                c1 = copy_coords(coords)
                c2 = copy_coords(coords)
                c1[axis] = i+1
                c2[axis] = i
                if space[c2[0]][c2[1]][c2[2]] is not None:
                    raise Exception(f"Trying to overwrite a node!!! {neg_dir = }, {coords = }, {axis = }, {i = }")
                space[c2[0]][c2[1]][c2[2]] = space[c1[0]][c1[1]][c1[2]]
                space[c1[0]][c1[1]][c1[2]] = None

        c = copy.deepcopy(coords)
        c[axis] -= 1
        if space[c[0]][c[1]][c[2]] is not None:
            raise Exception(f"Trying to overwrite a node!!!, {neg_dir = }, {coords = }, {axis = }")
        space[c[0]][c[1]][c[2]] = node['left_child']
        space[coords[0]][coords[1]][coords[2]] = node['right_child']
        new_nodes_num = count_nodes(space, space_size)
        if new_nodes_num <= nodes_num:
            raise Exception(f"Nodes number didn't increase: {nodes_num = }, {new_nodes_num = }")
    else:
        # Mode right node
        for i in range(coords[axis]+pos_dir, coords[axis]+1, -1):
            c1 = copy_coords(coords)
            c2 = copy_coords(coords)
            c1[axis] = i-1
            c2[axis] = i
            if space[c2[0]][c2[1]][c2[2]] is not None:
                raise Exception(f"Trying to overwrite a node!!! {neg_dir = }, {coords = }, {axis = }, {i = }")
            space[c2[0]][c2[1]][c2[2]] = space[c1[0]][c1[1]][c1[2]]
            space[c1[0]][c1[1]][c1[2]] = None

        c = copy.deepcopy(coords)
        c[axis] += 1
        space[c[0]][c[1]][c[2]] = node['right_child']
        space[coords[0]][coords[1]][coords[2]] = node['left_child']
        new_nodes_num = count_nodes(space, space_size)
        if new_nodes_num <= nodes_num:
            raise Exception(f"Nodes number didn't increase: {nodes_num = }, {new_nodes_num = }")
    return [c, coords]


def get_free_cell(space, new_coords, space_size):
    steps = [[0, i, -i] for i in range(1, space_size)]
    for rad in steps:
        for i in rad:
            for j in rad:
                for k in rad:
                    if ((new_coords[0] + i) >= space_size) or ((new_coords[1] + j) >= space_size) or ((new_coords[2] + k) >= space_size):
                        continue
                    if ((new_coords[0] + i) < 0) or ((new_coords[1] + j) < 0) or ((new_coords[2] + k) < 0):
                        continue
                    n = space[new_coords[0] + i][new_coords[1] + j][new_coords[2] + k]
                    if n is None:
                        return [new_coords[0] + i, new_coords[1] + j, new_coords[2] + k]
    for i in range(space_size):
        for j in range(space_size):
            for k in range(space_size):
                if space[i][j][k] is None:
                    return [i, j, k]
    raise Exception(f"Cannot find a free cell for {new_coords = }")


def move_node(space, coords, offset, space_size, force=False):
    ''' Returns new coords, set force to True to move a node to the same location (force it to free the location) '''
    node = space[coords[0]][coords[1]][coords[2]]
    new_coords = []
    for i in range(3):
        c = coords[i] + offset[i]
        if c >= space_size:
            c = space_size-1
        if c < 0:
            c = 0
        new_coords.append(c)
    # new_coords = [coords[0]+offset[0], coords[1]+offset[1], coords[2]+offset[2]]
    if space[new_coords[0]][new_coords[1]][new_coords[2]] is None:
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node
        space[coords[0]][coords[1]][coords[2]] = None
        return new_coords
    if coords == new_coords:
        if not force:
            return  coords  # no need to move a node to the same cell
    # Specified cell is not empty, look for a free cell around
    target = get_free_cell(space, new_coords, space_size)
    space[target[0]][target[1]][target[2]] = node
    space[coords[0]][coords[1]][coords[2]] = None
    return target


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
    move_node(space, [4, 2, 2], [1, 2, 2], space_size)
    move_node(space, [3, 2, 2], [1, 2, 2], space_size)
    move_node(space, [2, 2, 2], [1, 2, 2], space_size)
    for i in range(space_size):
        for j in range(space_size):
            for k in range(space_size):
                if space[i][j][k] is not None:
                    print(f"Node [{i}][{j}][{k}]")



if __name__ == "__main__":
    test()
