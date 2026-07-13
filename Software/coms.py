from os import wait

import serial
import sys 
import glob

from serial.serialposix import Serial

def list_serial_devices():
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')
    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result
    

def open_serial(port_name:str):
    com = serial.Serial(port_name, 115200, timeout=1)
    return com


def do_conf(port_name:str, conf:list[int]):
    com = serial.Serial(port_name, 115200, timeout=1)

    com.write(b"  conf\n")
    p = com.read_until(b"ACK").decode()
    print(p)
    com.write(bytes(conf))
    p = com.read_until(b"ACK").decode()
    print(p)
    return com

def do_cont(com:Serial, cont:list[int]):
    com.write(b"  cont\n")
    _ = com.read_until(b"ACK").decode()

    com.write(bytes(cont))
    _ = com.read_until(b"ACK").decode()

    return com


def do_go(com:Serial):
    com.write(b"  go\n")
    data = com.read_until(b"ACK").decode()
    com.close()
    return data

    
def diagnostic_test(port_name:str):
    com = serial.Serial(port_name, 115200, timeout=1)
    print("Sending : \"  abc\"")
    com.write(b"  abc\n")
    data = com.read_until(b"ACK").decode()
    print("Received: \"" + data + "\"")
    

    print("Sending : \"  go\"")
    com.write(b"  go\n")
    data = com.read_until(b"ACK").decode()
    print("Received: \"" + data + "\"")

    print("Sending : \"  conf\"")
    com.write(b"  conf\n")
    data = com.read_until(b"ACK").decode()
    print("Received: \"" + data + "\"")

    print("Sending : \"  [0,0,5,0,1,2,3,4,5,6,7,8,9,10]\"")
    com.write(bytes([0,0,5,0,1,2,3,4,5,6,7,8,9,10]))
    data = com.read_until(b"ACK").decode()
    print("Received: \"" + data + "\"")

    print("Sending : \"  go\"")
    com.write(b"  go\n")
    data = com.read_until(b"ACK").decode()
    print("Received: \"" + data + "\"")



    return
