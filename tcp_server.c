/***************************************/
/*              TCP server             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 2001
#define error(a,b) fprintf(stderr, a, b)

int main(int argc, char *argv[] )

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
    char on;
    char buffer[BUFSIZE+1];

   /* Inicializáció */
   on                     = 1;
   flags                  = 0;
   bytes                  = BUFSIZE;
   server_size            = sizeof server;
   client_size            = sizeof client;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT_NO);

   /* Socket létrehozása */
   serverSocket = socket(AF_INET, SOCK_STREAM, 0 );
   if (serverSocket < 0)
   {
      error("%s: Nem sikerült létrehozni a socketet!\n",argv[0]);
      exit(1);
   }

   /* Socket beállítása */
   setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Socket bindelése */
   err = bind(serverSocket, (struct sockaddr *) &server, server_size);
   if (err < 0)
   {
      error("%s: Nem sikerült bebindelni a socketet!\n",argv[0]);
      exit(2);
   }

   /* Várakozás */
   err = listen(serverSocket, 10);
   if (err < 0)
   {
      error("%s: Sikertelen várakozás!\n",argv[0]);
      exit(3);
   }

   /* Csatlakozás elfogadása */
   playerSocket1 = accept(serverSocket, (struct sockaddr *) &client, &client_size);
   if (playerSocket1 < 0)
   {
      error("%s: Nem sikerült csatlakozni a sockethez !\n",argv[0]);
      exit(4);
   }

   /* Klienstől való adat fogadása */
    while (1)
    {
    rcvsize=0;
    rcvsize = recv( playerSocket1, buffer, bytes, flags );
        if (rcvsize < 0)
        {
            error("%s: Nem sikerült adatot fogadni a klienstől!\n",argv[0]);
            exit(5);
        }

   /* Felhasználói felület */
        printf("%i byte lett fogadva a klienstől\n Üzenet: %s \n",
           rcvsize-1, buffer);
        if (strcmp(buffer, "kilépés"))
   /* Nyugtázás küldése a kliensnek */
        sprintf(buffer,"Adatok átadása rendben!");
        bytes = strlen(buffer) + 1;
        trnmsize = send(playerSocket1, buffer, bytes, flags);
        if (trnmsize < 0)
        {
            error("%s: Nem sikerült adatot küldeni a kliensnek!\n",argv[0]);
            exit(6);
        }

   /* Felhasználói felület */
        printf ("Sikeres nyugtázás küldése a kliensnek!\n");
        buffer[rcvsize-1]='\0';
    }
   /* Socketek bezárása és kilépés */
    close(playerSocket1);
    close(playerSocket2);
    close(serverSocket);
   exit(0);

}
