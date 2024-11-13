import random
import asyncio
import websockets
import threading
import time
import json
import sys
from nodes import move_node, divide_node, count_nodes

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

# genome = {
#     "move_to": [0, 0, 0],
#     "split_dir": 0,
#     "right_child": None,
#     "left_child": None,
#     "neuron_id": 0
# }


def assign_ids(nodes):
    leaf_counter = 0
    for node in nodes:
        if node['left_child'] is None and node['right_child'] is None:
            node['id'] = random.randint(0, 128)
            leaf_counter += 1
    print(f"{leaf_counter = }")


def create_node():
    node = {}
    node["move_to"] = [random.randint(-5, 5), random.randint(-5, 5), random.randint(-5, 5)]
    node["split_dir"] = random.randint(0, 2)
    node["right_child"] = None
    node["left_child"] = None
    node["id"] = 0
    return node


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
    return nodes[0]


def process_node(space, coords, space_size):
    node = space[coords[0]][coords[1]][coords[2]]
    if node is None:
        return False
    if (node['left_child'] is None) and (node['right_child'] is None):
        return False
    count = count_nodes(space, space_size)
    new_coords = move_node(space, coords, node["move_to"], space_size)
    node = space[new_coords[0]][new_coords[1]][new_coords[2]]
    expect_increase = False
    if (node['left_child'] is not None) and (node['right_child'] is not None):
        expect_increase = True
    divide_node(space, new_coords, space_size, 1)
    new_count = count_nodes(space, space_size)
    if expect_increase and (new_count <= count):
        raise Exception(f"Network size didn't increase: {count = }, {new_count = }")
    return True


def unfold_genome(space, root_cell, init_coords, space_size):
    space[init_coords[0]][init_coords[1]][init_coords[2]] = root_cell
    need_to_continue = True
    while need_to_continue:
        need_to_continue = False
        for i in range(space_size):
            for j in range(space_size):
                for k in range(space_size):
                    if process_node(space, [i, j, k], space_size):
                        need_to_continue = True

 
# Global variable to store the WebSocket connection
client_connection = None
async_loop = None
shutdown_event = None

space_nodes = {}

async def handler(websocket, path):
    global client_connection
    client_connection = websocket
    try:
        async for message in websocket:
            print(f"Received message: {message}")
            # await client_connection.send(json.dumps({'array': [1, 2, 3, 4, 5]}))
            await client_connection.send(json.dumps(space_nodes))
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
        # async_loop.run_until_complete(client_connection.send(data))
        async_loop.run_until_complete(client_connection.send(json.dumps(data)))
        # asyncio.run_coroutine_threadsafe(client_connection.send(data), async_loop)


def main():
    network_size = 512   # 32768
    space_size = 10
    init_coords = [int(space_size/2), int(space_size/2), int(space_size/2)]
    space =  [[[None for i in range(space_size)] for j in range(space_size)] for k in range(space_size)]

    if network_size > (space_size ** 3):
        print("Network is too large and does not fit into the given space")
        sys.exit()
    unfold_genome(space, create_network(network_size), init_coords, space_size)
    server_thread = threading.Thread(target=start_server)
    server_thread.start()
    node_count = 0
    try:
        for i in range(space_size):
            for j in range(space_size):
                for k in range(space_size):
                    if space[i][j][k] is not None:
                        node_count += 1
                        space_nodes[f"{i}-{j}-{k}"] = {}
                        space_nodes[f"{i}-{j}-{k}"]['coords'] = {}
                        space_nodes[f"{i}-{j}-{k}"]['coords']['x'] = i
                        space_nodes[f"{i}-{j}-{k}"]['coords']['y'] = j
                        space_nodes[f"{i}-{j}-{k}"]['coords']['z'] = k
                        space_nodes[f"{i}-{j}-{k}"]['color'] = '0x00FFFF'
        print(f"There are {node_count} nodes in the space")
        while True:
            time.sleep(1)
            # send_data({'array': [1, 2, 3, 4, 5]})
    except KeyboardInterrupt:
        async_loop.call_soon_threadsafe(shutdown_event.set)
        server_thread.join()
        print("Server stopped")


if __name__ == "__main__":
    main()
