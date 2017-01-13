#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IPV4_LENGTH 16		// buffer overflow risk when using inet_ntop, etc? look into this later


int main(int argc, char *argv[])
{
	int rv;

	if (argc != 3)
	{
		printf("usage: ./hp <local ip> <local port>\n");
		exit(1);
	}

	struct in_addr local_ipv4;
	rv = inet_pton(AF_INET, argv[1], &local_ipv4);
	if (rv != 1)
	{
		printf("failed to convert local ipv4 to network format\n");
		exit(1);
	}

	char local_ipv4_string[IPV4_LENGTH];
	inet_ntop( AF_INET, &(local_ipv4), local_ipv4_string, IPV4_LENGTH );
	if (local_ipv4_string == NULL)
	{
		printf("failed to convert local ipv4 to presentation format\n");
		exit(1);
	} 
	printf("listening on ip %s\n", local_ipv4_string);

	uint16_t local_port = (uint16_t) atoi(argv[2]);
	printf("listening on port %u\n", local_port);

	struct sockaddr_in local_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(local_port),
		.sin_addr = local_ipv4
	};
	socklen_t local_addr_size = (socklen_t) sizeof(local_addr);

	int listening_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listening_sock == -1)
	{
		printf("failed to create socket\n");
		exit(1);
	}
	int enable = 1;
	if (setsockopt(listening_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{    
		printf("setsockopt(SO_REUSEADDR) failed\n");
		exit(1);
	}

	rv = bind( listening_sock, (const struct sockaddr *) &(local_addr), local_addr_size );
	if (rv == -1)
	{
		printf("failed to bind socket\n");
		exit(1);
	}
	printf("bound listening socket\n");

	rv = listen( listening_sock, 5 );
	if (rv == -1)
	{
		printf("failed to listen on bound socket\n");
		exit(1);
	}
	printf("listened successfully\n");

	struct sockaddr_in remote_addr;
	socklen_t remote_addr_size = (socklen_t) sizeof(remote_addr);
	printf("waiting for connections...\n");
	int conn_sock = accept( listening_sock, (struct sockaddr *) &(remote_addr), &(remote_addr_size) );
	if (conn_sock == -1)
	{
		printf("failed to accept connections\n");
		exit(1);
	}
	rv = getpeername(conn_sock, (struct sockaddr *) &(remote_addr), &(remote_addr_size) );
	if (rv == -1)
	{
		printf("failed to get connected socket's info\n");
		exit(1);
	}
	char remote_ipv4_string[IPV4_LENGTH];
	inet_ntop( AF_INET, &(remote_addr.sin_addr), remote_ipv4_string, IPV4_LENGTH );
	if (remote_ipv4_string == NULL)
	{
		printf("failed to convert remote ipv4 to presentation format\n");
		exit(1);
	}
	printf("got connection from %s\n", remote_ipv4_string);


	return 0;
}
