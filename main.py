

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

def move_neuron(coords, new_coords):
    neuron = space[coords[0]][coords[1]][coords[2]]
    if neuron:
        space[new_coords[0]][new_coords[1]][new_coords[2]] = neuron
        space[coords[0]][coords[1]][coords[2]] = None