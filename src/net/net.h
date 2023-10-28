#ifndef NET_H
#define NET_H

#define INIT_ERROR    1
#define BINDING_ERROR 2
#define LISTEN_FAILED 3
#define SELECT_FAILED 4
#define ACCEPT_FAILED 5

int start_server(int port,int max_clients,int buffer_size);

#endif
