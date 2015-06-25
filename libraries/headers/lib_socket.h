/**
 * @file lib_socket.h
 * @author Kishore
 * @date June 24, 2015, 8:52 PM 
 */

/*Prototypes of functions.*/
int create_bind_socket(char *port);

void make_socket_nonblocking(int socket);

int connect_to_server(char *ip, char *port);