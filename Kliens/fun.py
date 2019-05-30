#!/usr/bin/env python3

import socket

def isValidIP(address):
    try:
        socket.inet_pton(socket.AF_INET, address)
    except AttributeError:
        try:
            socket.inet_aotn(address)
        except socket.error:
            return False
        return address.count('.') == 3
    except socket.error:
        return False

    return True

def usage():
    print("client [server IP] [server port]")

def instructions():
    print("----------------------------------------------------------------------------")
    print("                     HASZNÁLATI UTASÍTÁS")
    print("	A feladom paranccsal bármikor feladhatod a játékot.")
    print("	Általában a következő formában kell megadnod a lépésedet: SZÁM SZÁM")
    print("	ahol az első szám a sor a második pedig az oszlop száma.")
    print("----------------------------------------------------------------------------")
    print()

def vonal(n):
    print()
    for i in range(n):
        print("-", end='')
    print()

def szokoz(n):
    for i in range(n):
        print(" ", end='')

def printTablak(tabla1, tabla2):
    # feltételezzük hogy egyformák a dimenziók
    n = len(tabla1)
    m = len(tabla1[0])

    print("Te táblád:")
    szokoz(5)
    for i in range(n):
        print(str(i+1) + " ", end='')
        szokoz(2)
    vonal(4*n+5)
    for i in range(n):
        print(str(i+1) + " || " , end='')
        for j in range(m):
            print(tabla1[i][j], '| ', end='')
        vonal(4*n+5)

    print("\nSegédtábla (ellenfél):")
    szokoz(5)
    for i in range(n):
        print(str(i+1) + " ", end='')
        szokoz(2)
    vonal(4*n+5)
    for i in range(n):
        print(str(i+1) + " || " , end='')
        for j in range(m):
            print(tabla2[i][j], '| ', end='')
        vonal(4*n+5)

    print()
