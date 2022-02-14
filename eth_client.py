import socket


SRV_IP = '127.0.0.1'
SRV_PORT = 8765
SRV_ADDR = (SRV_IP, SRV_PORT)

BUFF_SIZE = 1024

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as cli:
    try:
        cli.connect(SRV_ADDR)
    except ConnectionRefusedError:
        print('Cannot connect')
        exit()
       
    while True:
        try:
            cli.send(str(input("input: ")).encode())
        except KeyboardInterrupt:
            print('Stop client')
            break
        except BrokenPipeError:
            print('Server might be down')
            break

