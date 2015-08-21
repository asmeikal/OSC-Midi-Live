import serial
import sys

def readOneMessage(s, n):
    """Whats goin on ere, u say? 6_6"""
    c = 0
    r = s.read(1)
    while r != b'\xFE':
        c += 1
        if c == (n+2) * 2:
            return 1
        r = s.read(1)
    c = 1
    r = s.read(1)
    if r != b'\xFE':
        return 1
    c += 1
    while c < (n+1) * 2:
        s.read(1)
        r = s.read(1)
        if r != b'\x00':
            return 1
        c += 2
    r = s.read(2)
    if r == b'\xEF\xEF':
        return 0
    else:
        return 1

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("usage: python3 python_logger.py [devname] [baudrate] [n_vars]")
        exit(1)
    try:
        ser = serial.Serial(sys.argv[1], sys.argv[2])    
    except Exception as e:
        exit(1)
    rv = readOneMessage(ser, int(sys.argv[3]))
    exit(rv)
