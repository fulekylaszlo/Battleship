/***************************************/
/*              UDP client             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 2001
#define error(a,b) fprintf(stderr, a, b)

int main(int argc, char *argv[] ){ // arg count, arg vector   

   /* Declarations */
   int fd;		                     // socket endpt	
   int flags;                      // rcv flags
   struct sockaddr_in server;	     // socket name (addr) of server 
   struct sockaddr_in client;      // socket name of client 
   int server_size;                // length of the socket addr. server 
   int client_size;                // length of the socket addr. client 
   int bytes;    	                 // length of buffer 
   int trnmsize;                   // transmitted bytes
   int ip;												 // ip address
   char on;                        // 
   char buffer[BUFSIZE+1];         // datagram dat buffer area
   char server_addr[16];           // server address	

   /* Initialization */
   on    = 1;
   flags = 0;
   server_size = sizeof server;
   client_size = sizeof client;
   sprintf(server_addr, "%s", argv[1]);
   ip = inet_addr(server_addr);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = ip;
   server.sin_port        = htons(PORT_NO);
   
   /* User interface */
   printf(" Message: ");
   scanf("%s",buffer);
   bytes=strlen(buffer)+1;

   /* Creating socket */
   fd = socket(AF_INET, SOCK_DGRAM, 0);
   if (fd < 0) {
      error("%s: Socket creation error.\n",argv[0]);
      exit(1);
   }

   /* Setting options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Sending data to the server */
   trnmsize = sendto(fd, buffer, bytes, flags, 
		(struct sockaddr *) &server, server_size);
   if (trnmsize <= 0) {
      error("%s: Sending error (0 byte sent).\n", argv[0]);
      exit(2);
   }
      
   /* Receive data from the server */
   rcvsize = recvfrom(fd, buffer, bytes, flags,
		  (struct sockaddr *) &client, &client_size);
   if (rcvsize < 0) {
      error("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(3);
   }
   /* User interface */
   printf ("%i bytes have been sent to server.\n", trnmsize-1);

   /* Closing sockets and quit */
   close(fd);
   exit(0);
} 
