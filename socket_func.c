
#include "socket_func.h"



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
