
#include "socket_func.h"



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

	char recv_buffer[1000];
	ssize_t bytes_received = recv( conn_sock, recv_buffer, (ssize_t) sizeof(recv_buffer), 0);
	if (bytes_received == -1)
	{
		printf("error in receiving bytes\n");
		return 1;
	}
	printf("received: %s\n", recv_buffer);

	return 0;
}
