/***************************************/
/*              UDP server             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define BUFSIZE 1024
#define PORT_NO 2001
#define error(a,b) fprintf(stderr, a, b)

int main(int argc, char *argv[] ){ // arg count, arg vector 

   /* Declarations */
   int fd;                         // socket endpt
   int flags;                      // rcv flags
   struct sockaddr_in server;      // socket name (addr) of server
   struct sockaddr_in client;	     // socket name of client
   int server_size;                // length of the socket addr. server
   int client_size;                // length of the socket addr. client
   int bytes;		           // length of buffer 
   int rcvsize;                    // received bytes
   int err;                        // error code
   char on;                        // 
   char buffer[BUFSIZE+1];         // datagram dat buffer area

   /* Initialization */
   on    = 1;
   flags = 0;
   bytes = BUFSIZE;
   server_size = sizeof server;
   client_size = sizeof client;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);

   /* Creating socket */
   fd = socket(AF_INET, SOCK_DGRAM, 0);
   if (fd < 0) {
      error("%s: Socket creation error.\n",argv[0]);
      exit(1);
      }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      error("%s: Cannot bind to the socket.\n", argv[0]);
      exit(2);
  }
      
   /* Sending data to the client */
   trnmsize = sendto(fd, buffer, bytes, flags, 
		(struct sockaddr *) &server, server_size);
   if (trnmsize <= 0) {
      error("%s: Sending error (0 byte sent).\n", argv[0]);
      exit(2);
   }

   /* Receive data from the client */
   rcvsize = recvfrom(fd, buffer, bytes, flags,
		  (struct sockaddr *) &client, &client_size);
   if (rcvsize < 0) {
      error("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(3);
   }

   /* User interface */
   printf ("%i bytes have been received from client.\n Message: %s \n",
	    rcvsize-1, buffer);

   /* Closing sockets and quit */
   close(fd);
   exit(0);
} 
