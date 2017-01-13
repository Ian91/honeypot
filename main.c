#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IPV4_LENGTH 16		// buffer overflow risk when using inet_ntop, etc? look into this later

int initListener( char *_local_ipv4_string, uint16_t _local_port )
{
	struct in_addr local_ipv4;
	int rv = inet_pton(AF_INET, _local_ipv4_string, &local_ipv4);
	if (rv != 1)
	{
		printf("failed to convert local ipv4 to network format\n");
		return -1;
	}
	printf("initializing listener on ip %s, port %u\n", _local_ipv4_string, _local_port);

	struct sockaddr_in local_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(_local_port),
		.sin_addr = local_ipv4
	};
	socklen_t local_addr_size = (socklen_t) sizeof(local_addr);

	int _listening_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_listening_sock == -1)
	{
		printf("failed to create listener socket\n");
		return -1;
	}
	int enable = 1;
	if (setsockopt(_listening_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{    
		printf("setsockopt(SO_REUSEADDR) failed on listener socket\n");
		return -1;
	}

	rv = bind( _listening_sock, (const struct sockaddr *) &(local_addr), local_addr_size );
	if (rv == -1)
	{
		printf("failed to bind listener socket\n");
		return -1;
	}

	rv = listen( _listening_sock, 5 );
	if (rv == -1)
	{
		printf("failed to listen on bound socket\n");
		return -1;
	}

	return _listening_sock;
}



int main(int argc, char *argv[])
{
	int rv;

	if (argc != 3)
	{
		printf("usage: ./hp <local ip> <local port>\n");
		return 1;
	}
	char *local_ipv4_string = argv[1];
	uint16_t local_port = (uint16_t) atoi(argv[2]);

	int listening_sock = initListener( local_ipv4_string, local_port);
	if (listening_sock == -1)
	{
		printf("error in initializing listener\n");
		return 1;
	}

	struct sockaddr_in remote_addr;
	socklen_t remote_addr_size = (socklen_t) sizeof(remote_addr);
	printf("waiting for connections...\n");
	int conn_sock = accept( listening_sock, (struct sockaddr *) &(remote_addr), &(remote_addr_size) );
	if (conn_sock == -1)
	{
		printf("failed to accept connections\n");
		return 1;
	}
	rv = getpeername(conn_sock, (struct sockaddr *) &(remote_addr), &(remote_addr_size) );
	if (rv == -1)
	{
		printf("failed to get connected socket's info\n");
		return 1;
	}
	char remote_ipv4_string[IPV4_LENGTH];
	inet_ntop( AF_INET, &(remote_addr.sin_addr), remote_ipv4_string, IPV4_LENGTH );
	if (remote_ipv4_string == NULL)
	{
		printf("failed to convert remote ipv4 to presentation format\n");
		return 1;
	}
	printf("got connection from %s\n", remote_ipv4_string);


	return 0;
}
