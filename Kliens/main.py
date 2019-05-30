#!/usr/bin/env python3

import sys
import socket
import time as t

from fun import *
from Torpedo import Torpedo

def main():
    argnum = len(sys.argv)

    if argnum < 3:
        usage()
        exit(1)

    try:
        serverIP = sys.argv[1]
        serverPort = int(sys.argv[2])
    except:
        usage()
        exit(1)

    if not isValidIP(serverIP):
        raise RuntimeError('Rossz IP')

    if serverPort > 65535 or serverPort < 1024:
        raise RuntimeError('Rossz port')

    Torpedo(serverIP, serverPort)

# Call main
if __name__ == '__main__':
    main()
