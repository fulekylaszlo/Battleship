/***************************************/
/*             TCP kliens              */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 2001
#define error(a,b) fprintf(stderr, a, b)

int main(int argc, char *argv[])

{

   /* Deklaráció */
    int serverSocket; /* fd */
    int playerSocket1; /* fdc */
    int playerSocket2;
    
    
   int flags;
   struct sockaddr_in server;
   struct sockaddr_in client;
   int server_size;
   int client_size;
   int bytes;
   int rcvsize;
   int trnmsize;
   int err;
   int ip;
   char on;
   char buffer[BUFSIZE+1];
   char server_addr[16];

   /* Inicializáció */
   on    = 1;
   flags = 0;
   server_size = sizeof server;
   client_size = sizeof client;
   sprintf(server_addr, "%s", argv[1]);
   ip = inet_addr(server_addr);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = ip;
   server.sin_port        = htons(PORT_NO);
    
    server.sin_addr.s_addr= inet_addr("127.0.0.1");
    server.sin_port = htons(PORT_NO);
   
   /* Felhasználói felület */
   printf(" Üzenet: ");
   scanf("%s",buffer);
   bytes = strlen(buffer)+1;

   /* Socket létrehozása */
   serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (serverSocket < 0)
   {
      error("%s: Nem sikerült létrehozni a socketet!\n",argv[0]);
      exit(1);
   }

   /* Socket beállítása */
   setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Csatlakozás a szerverhez */
   err = connect(serverSocket, (struct sockaddr *) &server, server_size);
   if (err < 0)
   {
      error("%s: Nem sikerült bebindelni a socketet!\n", argv[0]);
      exit(2);
   }
 while (1)
 {
   /* Adat küldése a szerver felé*/
   trnmsize = send(serverSocket, buffer, bytes, flags);
   if (trnmsize < 0)
   {
      error("%s: Nem sikerült adatot küldeni a szerver felé!\n", argv[0]);
      exit(3);
   }

   /* Felhasználói felület */
   printf(" Sikeresen felcsatlakozott a játékos!\n");

   /* Szervertől való adatok fogadása */
   rcvsize = recv( serverSocket, buffer, bytes, flags );
   if (rcvsize < 0)
   {
      error("%s: Nem sikerült adatot fogadni a szervertől!\n", argv[0]);
      exit(4);
   }

   /* Felhasználói felület */
   if (strcmp(buffer, "Adatok átadása rendben!"))
      printf("Sikeres nyugtázás fogadása a szervertől!\n");
   else
      printf("Nyugtázási hiba!");
     scanf("%s",buffer);
 }
   /* Socketek bezárása és kilépés */
   close(serverSocket);
   exit(0);
} 
