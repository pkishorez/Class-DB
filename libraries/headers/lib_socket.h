/**
 * @file lib_socket.h
 * @author Kishore
 * @date June 24, 2015, 8:52 PM 
 */

#include "lib_types.h"

int create_bind_socket(char *port);

void socket_make_nonblocking(int socket);

int socket_connect_to_server(char *ip, char *port);