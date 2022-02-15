from math import log2
import socket

SRV_IP = '127.0.0.1'
SRV_PORT = 8765
SRV_ADDR = (SRV_IP, SRV_PORT)

BUFF_SIZE = 1024

def ByteSize(data):
    if data == 0:
        return 1
    
    return int(log2(abs(data))//8)+1

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as cli:
    try:
        cli.connect(SRV_ADDR)
        pass
    except ConnectionRefusedError:
        print('Cannot connect')
        exit()
       
    while True:
        try:            
            req = int(input('Enter the Request\t 0: Get, 1: Set \n>> '))
            dev = int(input('Enter the Device ID\t 0: ETH, 1: UART \n>> '))
            dat = int(input('Enter the Data ID\t 0: State, 1: Routine, 2: Period \n>> '))
            arg = int(input('Enter the Argument\t \n>> '))

            cmd_list = [req, dev, dat, arg]
            cmd_byte = bytes()
            for data in cmd_list:
                cmd_byte += data.to_bytes(ByteSize(data), 'little', signed=True)

            n_data = len(cmd_byte)
            cmd_byte = n_data.to_bytes(ByteSize(n_data), 'little') + cmd_byte
        except ValueError:
            print('Invalid format')
            continue
        except KeyboardInterrupt:
            print('\nStop client')
            break

        try:
            print(f'Send Req: {cmd_list}')
            cli.send(cmd_byte)
            n_data = int.from_bytes(cli.recv(1), 'little')
            res_list = list(cli.recv(n_data+1))
            print(f'Recv Res: {res_list}')
            print()
        except BrokenPipeError:
            print('Server might be down')
            break
        