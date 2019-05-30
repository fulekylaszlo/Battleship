#!/usr/bin/env python3

import socket
import os
import sys

from fun import *

class Torpedo:
    def __init__(self, serverIP, serverPort):
        self.msgLen = 256 + 1

        self.serverIP = serverIP
        self.serverPort = serverPort

        self.tabla1 = [[None for _ in range(10)] for _ in range(10)]
        self.tabla2 = [[None for _ in range(10)] for _ in range(10)]

        self.utsoX = 1000
        self.utsoY = 1000

        # Create a socket and connect to it
        self.createSocket()
        self.connectToServer()

        # Run the main loop
        self.mainLoop()

        # Close the socket
        self.closeSocket()

    def createSocket(self):
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print("Socket created...")
        except socket.error as err:
            print("Socket failed to create! Message: " + str(err))
            exit(1)

    def connectToServer(self):
        try:
            self.s.connect((self.serverIP, self.serverPort))
            print("Connected to " + str(self.serverIP) + ":" + str(self.serverPort))
            instructions()

            for i in range(10):
                for j in range(10):
                    self.tabla1[i][j] = '~'
                    self.tabla2[i][j] = '~'

        except socket.error as err:
            print("Failed to connect! Message: " + str(err))
            exit(1)

    def mainLoop(self):
        while(True):
            received = self.receiveMsg()

            if len(received) == 0:
                break
                self.closeSocket()
                raise RuntimeError("Socket connection broken!")

            # print(received)

            if "TALALT" in received:
                print("Találat érte az ellenfél egyik hajóját!")
                self.tabla2[self.utsoX][self.utsoY] = '@'
                self.mutat()

            if "NYERT" in received:
                print(received)

            if "JATEKVEGE" in received:
                print("Játék vége. Kilépek!")
                self.closeSocket()
                sys.exit(0)

            if "UJJATEK" in received:
                del self.tabla1[:]
                del self.tabla2[:]
                self.tabla1 = [[None for _ in range(10)] for _ in range(10)]
                self.tabla2 = [[None for _ in range(10)] for _ in range(10)]

                for i in range(10):
                    for j in range(10):
                        self.tabla1[i][j] = '~'
                        self.tabla2[i][j] = '~'

                v = input("Szeretnél mégegy menetet? :3 (igen/nem) ")

                if "igen" in v:
                    self.sendMsg("ujra")
                    return self.mainLoop()
                else:
                    print("Tehát nem szeretnél.")
                    self.sendMsg("nem")

            if "VARAKOZAS" in received:
                print("Várakozunk a másik játékos csatlakozására...")

            if "HAJOID" in received:
                print("Te jössz! Meg kell adnod saját hajód (1x1)!")
                (n, m) = self.getHajo()

                if self.tabla1[int(n)][int(m)] != '~':
                    print("Ide már tettél hajót, adj meg egy másik helyet!")
                    (n, m) = self.getHajo()

                self.tabla1[int(n)][int(m)] = 'O'
                self.mutat()
                self.sendMsg(str(n+1) + " " + str(m+1))

            if "TAMADAS" in received:
                print("Támadj!")
                (n, m) = self.getHajo()
               
                self.utsoX = int(n)
                self.utsoY = int(m)
                self.tabla2[self.utsoX][self.utsoY] = 'N'
                self.mutat()
                self.sendMsg(str(n+1) + " " + str(m+1))

    def mutat(self):
        printTablak(self.tabla1, self.tabla2)

    def getHajo(self):
        be = input("Add meg a lépésedet: ")

        if "feladom" in be:
            self.sendMsg("feladom")
            self.closeSocket()
            sys.exit(0)

        try:
            n, m = be.split(' ')
            if int(n) not in range(0,11):
                return (self.getHajo())

            if int(m) not in range(0,11):
                return (self.getHajo())
        except (ValueError, TypeError):
            return (self.getHajo())

        return ( (int(n)-1), (int(m)-1) )

    def getStats(self):
        pass

    def printStats(self):
        pass

    def resign(self):
        print("Feladtad!")
        self.sendMsg("feladom")
        self.closeSocket()
        exit(0)

    def sendMsg(self, msg):
        msg += "\n"
        msg = msg.encode()
        sent = self.s.sendall(msg)
        if sent != None:
            raise RuntimeError("socket csatlakozás megszűnt")

    def receiveMsg(self):
        return self.s.recv(self.msgLen).decode()

    def closeSocket(self):
        print("Exiting now...")
        self.s.close()

