import socket
import base64
import hashlib
import threading
import select
import time

HOST = 'localhost'
PORT = 8765
stop_server_flag = False
server_socket = None
ws_server = None
clients = []


def handle_client(client_socket):
    global stop_server_flag, clients
    client_socket.setblocking(0)  # Set the socket to non-blocking mode
    request = client_socket.recv(1024).decode()
    headers = parse_headers(request)
    accept_key = generate_accept_key(headers['Sec-WebSocket-Key'])
    response = (
    'HTTP/1.1 101 Switching Protocols\r\n'
    'Upgrade: websocket\r\n'
    'Connection: Upgrade\r\n'
    f'Sec-WebSocket-Accept: {accept_key}\r\n\r\n'
    )
    client_socket.send(response.encode())
    while True:
        readable, _, _ = select.select([client_socket], [], [], 0.1)
        if readable:
            message = receive_message(client_socket)
            if message:
                print(f"Received message: {message}")
                # send_message(client_socket, message)
            else:
                break
        if stop_server_flag:
            break
    clients.remove(client_socket)
    client_socket.close()


def parse_headers(request):
    headers = {}
    lines = request.split('\r\n')
    for line in lines[1:]:
        if ': ' in line:
            key, value = line.split(': ', 1)
            headers[key] = value
    return headers


def generate_accept_key(key):
    GUID = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
    accept_key = key + GUID
    accept_key = hashlib.sha1(accept_key.encode()).digest()
    accept_key = base64.b64encode(accept_key).decode()
    return accept_key


def receive_message(client_socket):
    try:
        data = client_socket.recv(1024)
        if not data:
            return None
        payload_length = data[1] & 127
        if payload_length == 126:
            payload_length = int.from_bytes(data[2:4], byteorder='big')
            masks = data[4:8]
            message = data[8:8+payload_length]
        elif payload_length == 127:
            payload_length = int.from_bytes(data[2:10], byteorder='big')
            masks = data[10:14]
            message = data[14:14+payload_length]
        else:
            masks = data[2:6]
            message = data[6:6+payload_length]
            decoded_message = bytearray([message[i] ^ masks[i % 4] for i in range(len(message))])
        return decoded_message.decode()
    except Exception as e:
        print(f"Error receiving message: {e}")
    return None


def send_message(client_socket, message):
    encoded_message = message.encode()
    message_length = len(encoded_message)
    if message_length <= 125:
        header = bytearray([129, message_length])
    elif message_length <= 65535:
        header = bytearray([129, 126]) + message_length.to_bytes(2, byteorder='big')
    else:
        header = bytearray([129, 127]) + message_length.to_bytes(8, byteorder='big')
    client_socket.send(header + encoded_message)


def send_data(message):
    ''' Sends the message to all connected clients '''
    global clients
    for client in clients:
        send_message(client, message)


def run_server(ws_port):
    global stop_server_flag, server_socket, clients
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, ws_port))
    server_socket.listen(5)
    server_socket.setblocking(0)  # Set to non-blocking mode
    print(f"Server started on {HOST}:{ws_port}")
    client_servers = []
    while True:
        try:
            readable, _, _ = select.select([server_socket], [], [], 0.1)
            if readable:
                client_socket, addr = server_socket.accept()
                print(f"Connection from {addr}")
                clients.append(client_socket)
                client_handler = threading.Thread(target=handle_client, args=(client_socket,))
                client_handler.start()
                client_servers.append(client_handler)
        except socket.error:
            continue
        if stop_server_flag:
            print(f"{stop_server_flag = }")
            break
    for client_server in client_servers:
        client_server.join()


def start_server(ws_port):
    ws_server = threading.Thread(target=run_server, args=(ws_port,))
    ws_server.start()


def stop_server():
    global stop_server_flag, ws_server, server_socket
    print("Interrupt received, shutting down...")
    server_socket.close()
    stop_server_flag = True
    ws_server.join()


if __name__ == "__main__":
    start_server(PORT)
    try:
        while True:
            time.sleep(1)
            send_data("Hello world broadcast!")
    except KeyboardInterrupt:
        stop_server()
