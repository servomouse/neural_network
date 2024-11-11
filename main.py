import random
import asyncio
import websockets
import threading
import time
import json

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

ws_port = 8765
space_size = 100

space =  [[[None for i in range(space_size)] for j in range(space_size)] for k in range(space_size)]

genome = {
    "mov_x": 0,
    "mov_y": 0,
    "mov_z": 0,
    "split_dir": 'x',
    "right_child": None,
    "left_child": None,
    "neuron_id": 0
}

network_size = 32768

def assign_ids(nodes):
    for node in nodes:
        if node['left_child'] is None and node['right_child'] is None:
            node['id'] = random.randint(128)

def create_node():
    node = {}
    node["move_x"] = random.randint(-5, 5)
    node["move_y"] = random.randint(-5, 5)
    node["move_z"] = random.randint(-5, 5)
    dirs = ['x', 'y', 'z']
    node["split_dir"] = dirs[random.randint(3)]
    node["right_child"] = None
    node["left_child"] = None
    node["id"] = 0

def create_network(network_size):
    network_size = (network_size * 2) - 1
    nodes = [create_node() for _ in range(network_size)]
    for i in range(network_size):
        left_idx = 2 * i + 1
        right_idx = 2 * i + 2
        if left_idx < network_size:
            nodes[i]['left_child'] = nodes[left_idx]
        if right_idx < network_size:
            nodes[i]['right_child'] = nodes[right_idx]
    assign_ids(nodes)
    return nodes


init_coords = [int(space_size/2), int(space_size/2), int(space_size/2)]

def move_neuron(coords, new_coords):
    global space
    neuron = space[coords[0]][coords[1]][coords[2]]
    if neuron:
        space[new_coords[0]][new_coords[1]][new_coords[2]] = neuron
        space[coords[0]][coords[1]][coords[2]] = None


def move_node(coords, axis, direction):
    global space
    if direction == 'pos':
        if axis == 'x':
            if space[coords[0]+1][coords[1]][coords[2]] is not None:
                move_node([[coords[0]+1], [coords[1]], [coords[2]]], axis, direction)
            space[coords[0]+1][coords[1]][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'y':
            if space[coords[0]][coords[1]+1][coords[2]] is not None:
                move_node([[coords[0]], [coords[1]+1], [coords[2]]], axis, direction)
            space[coords[0]][coords[1]+1][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'z':
            if space[coords[0]][coords[1]][coords[2]+1] is not None:
                move_node([[coords[0]], [coords[1]], [coords[2]+1]], axis, direction)
            space[coords[0]][coords[1]][coords[2]+1] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
    elif direction == 'neg':
        if axis == 'x':
            if space[coords[0]-1][coords[1]][coords[2]] is not None:
                move_node([[coords[0]-1], [coords[1]], [coords[2]]], axis, direction)
            space[coords[0]-1][coords[1]][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'y':
            if space[coords[0]][coords[1]-1][coords[2]] is not None:
                move_node([[coords[0]], [coords[1]-1], [coords[2]]], axis, direction)
            space[coords[0]][coords[1]-1][coords[2]] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None
        elif axis == 'z':
            if space[coords[0]][coords[1]][coords[2]-1] is not None:
                move_node([[coords[0]], [coords[1]], [coords[2]-1]], axis, direction)
            space[coords[0]][coords[1]][coords[2]-1] = space[coords[0]][coords[1]][coords[2]]
            space[coords[0]][coords[1]][coords[2]] = None


def process_node(coords):
    global space
    node = space[coords[0]][coords[1]][coords[2]]
    new_coords = [coords[0] + node['mov_x'], coords[1] + node['mov_y'], coords[2] + node['mov_z']]
    if node["left_child"] is not None and node["right_child"] is not None:
        space[coords[0]][coords[1]][coords[2]] = None
        if node['split_dir'] == 'x':
            if space[new_coords[0]+1][new_coords[1]][new_coords[2]] is not None:
                move_node([new_coords[0]+1, new_coords[1], new_coords[2]], 'x', 'pos')
            space[new_coords[0]+1][new_coords[1]][new_coords[2]] = node['left_child']
            if space[new_coords[0]-1][new_coords[1]][new_coords[2]] is not None:
                move_node([new_coords[0]-1, new_coords[1], new_coords[2]], 'x', 'neg')
            space[new_coords[0]-1][new_coords[1]][new_coords[2]] = node['right_child']
            process_node([new_coords[0]+1, new_coords[1], new_coords[2]])
            process_node([new_coords[0]-1, new_coords[1], new_coords[2]])
        if node['split_dir'] == 'y':
            if space[new_coords[0]][new_coords[1]+1][new_coords[2]] is not None:
                move_node([new_coords[0], new_coords[1]+1, new_coords[2]], 'y', 'pos')
            space[new_coords[0]][new_coords[1]+1][new_coords[2]] = node['left_child']
            if space[new_coords[0]][new_coords[1]-1][new_coords[2]] is not None:
                move_node([new_coords[0], new_coords[1]-1, new_coords[2]], 'y', 'neg')
            space[new_coords[0]][new_coords[1]-1][new_coords[2]] = node['right_child']
            process_node([new_coords[0], new_coords[1]+1, new_coords[2]])
            process_node([new_coords[0], new_coords[1]-1, new_coords[2]])
        if node['split_dir'] == 'z':
            if space[new_coords[0]][new_coords[1]][new_coords[2]+1] is not None:
                move_node([new_coords[0], new_coords[1], new_coords[2]+1], 'z', 'pos')
            space[new_coords[0]][new_coords[1]][new_coords[2]+1] = node['left_child']
            if space[new_coords[0]][new_coords[1]][new_coords[2]-1] is not None:
                move_node([new_coords[0], new_coords[1], new_coords[2]-1], 'z', 'neg')
            space[new_coords[0]][new_coords[1]][new_coords[2]-1] = node['right_child']
            process_node([new_coords[0], new_coords[1], new_coords[2]+1])
            process_node([new_coords[0], new_coords[1], new_coords[2]-1])
    elif node["left_child"] is not None:
        space[coords[0]][coords[1]][coords[2]] = None
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node["left_child"]
        process_node([new_coords[0], new_coords[1], new_coords[2]])
    elif node["right_child"] is not None:
        space[coords[0]][coords[1]][coords[2]] = None
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node["right_child"]
        process_node([new_coords[0], new_coords[1], new_coords[2]])
    else:
        space[new_coords[0]][new_coords[1]][new_coords[2]] = node


def unfold_genome(genome, init_coords):
    global space
    space[init_coords[0]][init_coords[1]][init_coords[2]] = genome
    process_node(init_coords)

 
# Global variable to store the WebSocket connection
client_connection = None
async_loop = None
shutdown_event = None

async def handler(websocket, path):
    global client_connection
    client_connection = websocket
    try:
        async for message in websocket:
            print(f"Received message: {message}")
            # await client_connection.send(json.dumps({'array': [1, 2, 3, 4, 5]}))
    except websockets.ConnectionClosed:
        print("Client disconnected")
    finally:
        client_connection = None


async def run_server():
    global async_loop, shutdown_event
    shutdown_event = asyncio.Event()
    async_loop = asyncio.get_running_loop()
    server = await websockets.serve(handler, "localhost", ws_port)
    await shutdown_event.wait()
    server.close()
    await server.wait_closed()


def start_server():
    asyncio.run(run_server())


def send_data(data):
    global client_connection, async_loop
    if client_connection:
        print(f"Sending {data = }")
        async_loop.run_until_complete(client_connection.send(json.dumps(data)))
        # asyncio.run_coroutine_threadsafe(client_connection.send(data), async_loop)


def main():
    server_thread = threading.Thread(target=start_server)
    server_thread.start()
    try:
        # unfold_genome(create_network(network_size), init_coords)
        while True:
            time.sleep(1)
            send_data({'array': [1, 2, 3, 4, 5]})
    except KeyboardInterrupt:
        async_loop.call_soon_threadsafe(shutdown_event.set)
        server_thread.join()
        print("Server stopped")


if __name__ == "__main__":
    main()
