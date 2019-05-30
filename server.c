#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFSIZE 1024
#define PORT_NO 25000


int 
main()
{
	int		serverSocket;
	int		playerSocket1;
	int		playerSocket2;
	int		flags;
	struct sockaddr_in server;
	//c’minform‡ci— (ip + port)
		struct sockaddr_in client;
	int		server_size;
	int		client_size;
	int		bytes;
	int		fogadottmeret;
	int		atvittmeret;
	int		err;
	char		on;
	char		buffer    [BUFSIZE + 1];
	char		palya1    [10][10];
	char		palya2    [10][10];
	int		hajo1     , hajo2;
	bool		p1win    , p2win;

	on = 1;
	flags = 0;
	bytes = BUFSIZE;
	server_size = sizeof server;
	client_size = sizeof client;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT_NO);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
	if (serverSocket < 0) {
		fprintf(stderr, "Hiba! Nem tudtuk letrehozni a szerver socketet!\n");
		exit(1);
	}
	
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
	setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

	err = bind(serverSocket, (struct sockaddr *)&server, server_size);
	err = listen(serverSocket, 2);
	
    playerSocket1 = accept(serverSocket, (struct sockaddr *)&client, (socklen_t *) & client_size);
	sprintf(buffer, "VARAKOZAS\n");
	bytes = strlen(buffer);
	atvittmeret = send(playerSocket1, buffer, bytes, flags);
	playerSocket2 = accept(serverSocket, (struct sockaddr *)&client, (socklen_t *) & client_size);

	while (1) {
		p1win = false, p2win = false;
        
        // Null‡zzuk a t‡bl‡zokat
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				palya1[i][j] = '~';
			}
		}
        
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				palya2[i][j] = '~';
			}
		}
        
        // 1. kliens
		sprintf(buffer, "TABLA\n");
		bytes = strlen(buffer);
		atvittmeret = send(playerSocket1, buffer, bytes, flags);
		sprintf(buffer, "%s\n", palya1[0]);
		bytes = strlen(buffer);
		atvittmeret = send(playerSocket1, buffer, bytes, flags);

        // 2. kliens
		sprintf(buffer, "TABLA\n");
		bytes = strlen(buffer);
		atvittmeret = send(playerSocket2, buffer, bytes, flags);
		sprintf(buffer, "%s\n", palya2[0]);
		bytes = strlen(buffer);
		atvittmeret = send(playerSocket2, buffer, bytes, flags);
        
		hajo1 = 0;
		hajo2 = 0;
		
        char		feltolt1  [4];
		char		feltolt2  [4];
        int		x         , y;
		
        for (int i = 0; i < 5; ++i) {
			sprintf(buffer, "HAJOID\n");
			bytes = strlen(buffer);
		
            atvittmeret = send(playerSocket1, buffer, bytes, flags);
			fogadottmeret = recv(playerSocket1, feltolt1, 8, flags);
			
            feltolt1[fogadottmeret - 1] = '\0';
			
            x = (int)feltolt1[0] - '1';
			y = (int)feltolt1[2] - '1';
			
            palya1[x][y] = 'H';
		}


		for (int i = 0; i < 5; ++i) {
			sprintf(buffer, "HAJOID\n");
			bytes = strlen(buffer);
			
            atvittmeret = send(playerSocket2, buffer, bytes, flags);
			fogadottmeret = recv(playerSocket2, feltolt2, 8, flags);
			
            x = (int)feltolt2[0] - '1';
			y = (int)feltolt2[2] - '1';
			
            palya2[x][y] = 'H';
		}

        int		count = 0;
		
        for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (palya1[i][j] == 'H') {
					count++;
				}
		
                if (count != 5) {
					p2win = true;
					break;
				}
			}
		}

        for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (palya2[i][j] == 'H') {
					count++;
				}
				
                if (count != 5) {
					p1win = true;
					break;
				}
			}
		}

		char		tamad1    [8];
		char		tamad2    [8];
		int		p1score = 5, p2score = 5;

		while (1) {

			sprintf(buffer, "TAMADAS\n");
			bytes = strlen(buffer);
			atvittmeret = send(playerSocket1, buffer, bytes, flags);
			fogadottmeret = recv(playerSocket1, tamad1, 8, flags);
			tamad1[fogadottmeret - 1] = '\0';
		
            x = (int)tamad1[0] - '1';
			y = (int)tamad1[2] - '1';
			
            if (palya2[x][y] == 'H') {
				palya2[x][y] = '@';
				p2score--;

				sprintf(buffer, "TALALT\n");
				bytes = strlen(buffer);
				atvittmeret = send(playerSocket1, buffer, bytes, flags);
			} 
			
            if (p1score == 0) {
				p2win = true;
				break;
			} else if (p2score == 0) {
				p1win = true;
				break;
			} else if (strcmp(tamad1, "feladom") == 0) {
				p2win = true;
				break;
			}
			
            sprintf(buffer, "TAMADAS\n");
			bytes = strlen(buffer);
			atvittmeret = send(playerSocket2, buffer, bytes, flags);
			fogadottmeret = recv(playerSocket2, tamad2, 8, flags);
			tamad2[fogadottmeret - 1] = '\0';
			
            x = (int)tamad2[0] - '1';
			y = (int)tamad2[2] - '1';
			
            if (palya1[x][y] == 'H') {
				palya1[x][y] = '@';
				p1score--;
				
				sprintf(buffer, "TALALT\n");
				bytes = strlen(buffer);
				atvittmeret = send(playerSocket2, buffer, bytes, flags);
			}

			if (p1score == 0) {
				p2win = true;
				break;
			} else if (p2score == 0) {
				p1win = true;
				break;
			} else if (strcmp(tamad2, "feladom") == 0) {
				p1win = true;
				break;
			}
		}
	
        char		p1dontes  [5];
		char		p2dontes  [5];
		
        sprintf(buffer, "UJJATEK\n");
		bytes = strlen(buffer);

		atvittmeret = send(playerSocket1, buffer, bytes, flags);
		fogadottmeret = recv(playerSocket1, p1dontes, 5, flags);
		p1dontes[fogadottmeret - 1] = '\0';

		atvittmeret = send(playerSocket2, buffer, bytes, flags);
		fogadottmeret = recv(playerSocket2, p2dontes, 5, flags);
		p2dontes[fogadottmeret - 1] = '\0';

		if (!(strcmp(p1dontes, "ujra") == 0 && strcmp(p2dontes, "ujra") == 0)) {
			break;
		}
	}

    sprintf(buffer, "JATEKVEGE\n");
	bytes = strlen(buffer);
	atvittmeret = send(playerSocket1, buffer, bytes, flags);
	atvittmeret = send(playerSocket2, buffer, bytes, flags);

	if (p1win == true) {
		sprintf(buffer, "AZ 1. JATEKOS NYERT!\n");
	} else if (p2win == true) {
		sprintf(buffer, "A 2. JATEKOS NYERT!\n");
	}

    bytes = strlen(buffer);
	atvittmeret = send(playerSocket1, buffer, bytes, flags);
	atvittmeret = send(playerSocket2, buffer, bytes, flags);

	close(playerSocket1);
	close(playerSocket2);
	close(serverSocket);
}
