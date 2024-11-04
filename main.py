

# neuron = {
#     "mov_x": 0,
#     "mov_y": 0,
#     "mov_z": 0,
#     "rot_x": 0,
#     "rot_y": 0,
#     "rot_z": 0,
#     "right_child": None,
#     "left_child": None,
#     "neuron_id": 0
# }

space =  [[[None for i in range(100)] for j in range(100)] for k in range(100)]

genome = {
    "mov_x": 0,
    "mov_y": 0,
    "mov_z": 0,
    "split_dir": 'x',
    "right_child": None,
    "left_child": None,
    "neuron_id": 0
}

init_coords = [50, 50, 50]

def move_neuron(coords, new_coords):
    neuron = space[coords[0]][coords[1]][coords[2]]
    if neuron:
        space[new_coords[0]][new_coords[1]][new_coords[2]] = neuron
        space[coords[0]][coords[1]][coords[2]] = None


def move_node(space, coords, axis, direction):
    if direction == 'pos':
        if axis == 'x':
            if space[coords[0]+1][coords[1]][coords[2]] is not None:
                move_node(space, [[coords[0]+1], [coords[1]], [coords[2]]], axis, direction)
            space[coords[0]+1][coords[1]][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'y':
            if space[coords[0]][coords[1]+1][coords[2]] is not None:
                move_node(space, [[coords[0]], [coords[1]+1], [coords[2]]], axis, direction)
            space[coords[0]][coords[1]+1][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'z':
            if space[coords[0]][coords[1]][coords[2]+1] is not None:
                move_node(space, [[coords[0]], [coords[1]], [coords[2]+1]], axis, direction)
            space[coords[0]][coords[1]][coords[2]+1] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
    elif direction == 'neg':
        if axis == 'x':
            if space[coords[0]-1][coords[1]][coords[2]] is not None:
                move_node(space, [[coords[0]-1], [coords[1]], [coords[2]]], axis, direction)
            space[coords[0]-1][coords[1]][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'y':
            if space[coords[0]][coords[1]-1][coords[2]] is not None:
                move_node(space, [[coords[0]], [coords[1]-1], [coords[2]]], axis, direction)
            space[coords[0]][coords[1]-1][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'z':
            if space[coords[0]][coords[1]][coords[2]-1] is not None:
                move_node(space, [[coords[0]], [coords[1]], [coords[2]-1]], axis, direction)
            space[coords[0]][coords[1]][coords[2]-1] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None


def process_node(space, coords):
    node = space[coords[0]][coords[1]][coords[2]]
    new_coords = [coords[0] + node['mov_x'], coords[1] + node['mov_y'], coords[2] + node['mov_z']]
    if node["left_child"] is not None and node["right_child"] is not None:
        space[coords[0]][coords[1]][coords[2]] = None
        if node['split_dir'] == 'x':
            if space[new_coords[0]+1][new_coords[1]][new_coords[2]] is not None:
                move_node(space, [new_coords[0]+1, new_coords[1], new_coords[2]], 'x', 'pos')
            space[new_coords[0]+1][new_coords[1]][new_coords[2]] = node['left_child']
            if space[new_coords[0]-1][new_coords[1]][new_coords[2]] is not None:
                move_node(space, [new_coords[0]-1, new_coords[1], new_coords[2]], 'x', 'neg')
            space[new_coords[0]-1][new_coords[1]][new_coords[2]] = node['right_child']
            process_node(space, [new_coords[0]+1, new_coords[1], new_coords[2]])
            process_node(space, [new_coords[0]-1, new_coords[1], new_coords[2]])
        if node['split_dir'] == 'y':
            if space[new_coords[0]][new_coords[1]+1][new_coords[2]] is not None:
                move_node(space, [new_coords[0], new_coords[1]+1, new_coords[2]], 'y', 'pos')
            space[new_coords[0]][new_coords[1]+1][new_coords[2]] = node['left_child']
            if space[new_coords[0]][new_coords[1]-1][new_coords[2]] is not None:
                move_node(space, [new_coords[0], new_coords[1]-1, new_coords[2]], 'y', 'neg')
            space[new_coords[0]][new_coords[1]-1][new_coords[2]] = node['right_child']
            process_node(space, [new_coords[0], new_coords[1]+1, new_coords[2]])
            process_node(space, [new_coords[0], new_coords[1]-1, new_coords[2]])
        if node['split_dir'] == 'z':
            if space[new_coords[0]][new_coords[1]][new_coords[2]+1] is not None:
                move_node(space, [new_coords[0], new_coords[1], new_coords[2]+1], 'z', 'pos')
            space[new_coords[0]][new_coords[1]][new_coords[2]+1] = node['left_child']
            if space[new_coords[0]][new_coords[1]][new_coords[2]-1] is not None:
                move_node(space, [new_coords[0], new_coords[1], new_coords[2]-1], 'z', 'neg')
            space[new_coords[0]][new_coords[1]][new_coords[2]-1] = node['right_child']
            process_node(space, [new_coords[0], new_coords[1], new_coords[2]+1])
            process_node(space, [new_coords[0], new_coords[1], new_coords[2]-1])
    elif node["left_child"] is not None:
        space[coords[0]][coords[1]][coords[2]] = None
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node["left_child"]
        process_node(space, [new_coords[0], new_coords[1], new_coords[2]])
    elif node["right_child"] is not None:
        space[coords[0]][coords[1]][coords[2]] = None
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node["right_child"]
        process_node(space, [new_coords[0], new_coords[1], new_coords[2]])
    else:
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node


def unfold_genome(genome, init_coords):
    space[init_coords[0]][init_coords[1]][init_coords[2]] = genome
    process_node(space, init_coords)


if __name__ == '__main__':
    unfold_genome(genome, init_coords)