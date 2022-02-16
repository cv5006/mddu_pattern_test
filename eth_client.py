from math import log2
import socket

SRV_IP = '127.0.0.1'
SRV_PORT = 8765
SRV_ADDR = (SRV_IP, SRV_PORT)

reqs = {0:'Get', 1:'Set'}
devs = {0:'ETH', 1:'UART'}
dats = {0:'State', 1:'Routines', 2:'Period'}

def DictString(dict:dict):
    out_str = ''
    for k, v in dict.items():
        out_str += f'{str(k)}: {v}, '

    return out_str

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
       
    i = 0
    while True:
        i += 1
        print(f'------------ SendRecv #{i} ------------')
        try:
            req = int(input(f'Enter the Request\t {DictString(reqs)} \n>> '))
            dev = int(input(f'Enter the Device ID\t {DictString(devs)} \n>> '))
            dat = int(input(f'Enter the Data ID\t {DictString(dats)} \n>> '))
            if reqs[req] == 'Set':
                arg = int(input(f'Enter the Argument\t \n>> '))
            else:
                arg = 0

            cmd_list = [req, dev, dat, arg]
            cmd_byte = bytes()
            for data in cmd_list:
                cmd_byte += data.to_bytes(ByteSize(data), 'little', signed=True)

            n_data = len(cmd_byte)
            cmd_byte = n_data.to_bytes(ByteSize(n_data), 'little') + cmd_byte
        except ValueError:
            print('Invalid format')
            print()
            continue
        except KeyboardInterrupt:
            print('\nStop client')
            break

        try:
            print()
            print(f'Send Req: {cmd_list}')

            cli.send(cmd_byte)
            n_data = int.from_bytes(cli.recv(1), 'little')
            res_list = list(cli.recv(n_data+1))

            print(f'Recv Res: {res_list}')
            print()

            result = 'Success' if (res_list[0] == 0) else 'Failed'
            print(f'Request   Result : {result}')
            print(f'Responsed Device : {devs[res_list[1]]}')
            print(f'Responsed Data ID: {dats[res_list[2]]}')
            print(f'Returned  Data   : {res_list[3:]}')
            print()
        except BrokenPipeError:
            print('Server might be down')
            break
        