#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "network.h"

int create_connection(int32_t host, int32_t port)
{
    struct sockaddr_in server;
    
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0){
        return 0;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(host);
	server.sin_port = htons(port);

    if (connect(clientSocket , (struct sockaddr *)&server , sizeof(server)) < 0){
        return 0;
    }
    return clientSocket;
}

int32_t network_listen(int32_t port)
{
    struct sockaddr_in server, client;

    int serverSocket = socket(AF_INET, SOCK_STREAM , 0);

	if (serverSocket < 0){
		return 0;
	} 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

    if(bind(serverSocket,(struct sockaddr *)&server, sizeof(server)) < 0){
        return 0;
    }
    if(listen(serverSocket , MAX_CLIENTS) < 0){
        return 0;
    }
    int clientLength = sizeof(client);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&clientLength);
    
    if (clientSocket < 0){
        printf("error in creating socket\n");
        return 0;
    }

    return clientSocket;
}

void network_send(int32_t netref,int32_t val)
{
    char message[1];
    message[0] = (char)val;
	if (send(netref, message, 1, 0) < 0 ){
        printf("ERROR\n");
    }
}

char network_recieve(int32_t netref)
{
    char message[1];
    if(recv(netref, message, 1, 0) < 0){
        printf("ERROR\n");
    }
    return message[0];
}

void close_connection(int32_t netref)
{
    close(netref);
}
