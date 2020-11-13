#ifndef NETWORK_H
#define NETWORK_H

#include "ijvm.h"

#define OP_NETBIND      ((uint8_t) 0xE1)
#define OP_NETCONNECT   ((uint8_t) 0xE2)
#define OP_NETIN        ((uint8_t) 0xE3)
#define OP_NETOUT       ((uint8_t) 0xE4)
#define OP_NETCLOSE     ((uint8_t) 0xE5)
#define MAX_CLIENTS     42             

/**
 * Creates a new network connection to a server.
 * @param host The ip address of the server as an integer
 * @param port The port of the server
 * @returns The netref identifier of the new socket, or
 *          0 on failure
 **/
int32_t create_connection(int32_t host, int32_t port);

/**
 * Creates a new network connection for recieving data
 * @param port The port in which the socket listens to traffic
 * @returns The indedifier of the listening socket(netref), or
 *          0  on failure
 **/
int32_t network_listen(int32_t port);

/**
 * Sends a character over an active network connection,
 * identified by the netref
 * @param netref The identifier of an active socket
 * @param val The chararacter to be sent
 **/
void network_send(int32_t netref,int32_t val);

/**
 * Recieves a character from an active network connection,
 * identified by the netref
 * @param netref The identifier of an active socket
 * @returns the character tha was recieved
 **/
char network_recieve(int32_t netref);

/**
 * Closes a network connection identified by the netref
 **/
void close_connection(int32_t netref);

#endif
