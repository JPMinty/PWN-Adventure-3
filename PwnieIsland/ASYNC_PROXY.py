import threading
import socket
import time

# Define proxy and server variables
prox_host = 'localhost'
prox_port = 3002
serv_host = '<ENTER SERVER IP HERE>'
serv_port = 3002

# Create socket and bind to proxy
prox_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
prox_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
prox_socket.bind((prox_host, prox_port))
prox_socket.listen(10)

serv_queue = []
client_queue = []

print(f"Proxy active: {prox_host}:{prox_port}")

def server_handler(client_socket,server_socket):
    global serv_queue
    while True:
        server_data = client_socket.recv(4096)
        if not(server_data.hex().startswith("6d76")):
            print(f"Client > Server: {server_data.hex()}")
        if len(server_data) >= 1:
            if len(serv_queue)>0:
                data = serv_queue.pop()
                if not (data.hex().startswith("6d76")):
                    print(f"Client > Server: {data.hex()}")
                server_socket.sendall(data)
            server_socket.sendall(server_data)

    client_socket.close()
    server_socket.close()
            

def game_handler(client_socket,server_socket):
    global client_queue
    while True:
        client_data = server_socket.recv(4096)
        if not (client_data.hex().startswith("6d76")):
            print(f"Server > Client: {client_data.hex()}")
        if len(client_data) >= 1:
            if len(client_queue)>0:
                data = client_queue.pop()
                if not (data.hex().startswith("6d76")):
                    if not (data.hex().startswith("0000")):
                        print(f"Server > Client: {data.hex()}")
                client_socket.sendall(data)
            client_socket.sendall(client_data)

    client_socket.close()
    server_socket.close()
    
def connect_and_inj_handler(client_socket):
    serv_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv_socket.connect((serv_host, serv_port))

    # Receive remote server data
    client_data = client_socket.recv(4096)

    # Relay received server data to game client and print
    serv_socket.sendall(client_data)

    # Receive response from client
    remote_data = serv_socket.recv(4096)

    #Send data back to the remote server  
    client_socket.sendall(remote_data)

    game_thread = threading.Thread(target=game_handler, args=(client_socket,serv_socket))
    game_thread.start()
    server_thread = threading.Thread(target=server_handler, args=(client_socket,serv_socket))
    server_thread.start()

    while True:
        try:
            inject_pkt = input('> ')
            if (inject_pkt.startswith("openblocky")):
                hex_data = '0x30310600537461676531000000000300030000'
                remote_data = bytes.fromhex(hex_data[2:])
                client_queue.append(remote_data)
                hex_data = '0x303106005374616765320300000004000f0000'
                remote_data = bytes.fromhex(hex_data[2:])
                client_queue.append(remote_data)
                hex_data = '0x30310600537461676533000000000600030000'
                remote_data = bytes.fromhex(hex_data[2:])
                client_queue.append(remote_data)
                hex_data = '0x30310600537461676534020000000800770000'
                remote_data = bytes.fromhex(hex_data[2:])
                client_queue.append(remote_data)
            if (inject_pkt.startswith("blockyfinal")):
                for x1 in range(0x60, 0xff+1):
                    for x2 in range(0x60, 0xff+1):
                        for x3 in range(0x60, 0xff+1):
                            for x4 in range(0x60, 0xff+1):
                                brute_hex1 = hex(x1)
                                brute_hex2 = hex(x2)
                                brute_hex3 = hex(x3)
                                brute_hex4 = hex(x4)
                                hex_data = '0x30310a0046696e616c5374616765'+f"{brute_hex4[2:]}"+f"{brute_hex3[2:]}"+f"{brute_hex2[2:]}"+f"{brute_hex1[2:]}"
                                remote_data = bytes.fromhex(hex_data[2:])
                                print(f"Queue: {remote_data.hex()}")
                                serv_queue.append(remote_data)
                                if hex_data == '0x30310a0046696e616c5374616765faab8f69':
                                    print("SOLVED BLOCKY CHALLENGE")
                                    break
            if (inject_pkt.startswith("eggs")):
                hex_data = '0x63700900476f6c64656e45676709000000'
                remote_data = bytes.fromhex(hex_data[2:])
                client_queue.append(remote_data)
            if (inject_pkt.startswith("egghunter")):
                # egg 1
                hex_data='0x6d7600aac3c6004a8d460000824388059c530000000065650b000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 2
                hex_data='0x6d76007249c7001f6fc700e09c4588059c530000000065650c000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 3
                hex_data='0x6d760080bf46001988470030264588059c530000000065650d000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 4
                hex_data='0x6d7600256c47000288c600b0374588059c530000000065650e000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 5
                hex_data='0x6d760040be4400d869460070db4588059c530000000065650f000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 6
                hex_data='0x6d7600503546002c4dc60080cd4388059c5300000000656510000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 7
                hex_data='0x6d7680ed8dc7003f51c700a0cd4488059c5300000000656511000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 8
                hex_data='0x6d7600143d4700aadb460000304488059c5300000000656512000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 9
                hex_data='0x6d7600c97e470060b3c500009a4588059c5300000000656513000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)
                time.sleep(2)
                # egg 10
                hex_data='0x6d7600a02dc5006c2cc60020244688059c5300000000656514000000'
                remote_data = bytes.fromhex(hex_data[2:])
                serv_queue.append(remote_data)                

        except Exception as exception:
            print(exception)

    #Close the sockets
    serv_socket.close()
    client_socket.close()

while True:
    client_socket, client_address = prox_socket.accept()
    print(f"Connection from: {client_address[0]}:{client_address[1]}")

    # Create thread for new client
    proxy_thread = threading.Thread(target=connect_and_inj_handler, args=(client_socket,))
    proxy_thread.start()
