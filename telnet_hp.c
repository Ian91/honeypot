
#include "socket_func.h"
#include "telnet_hp.h"



int runTelnetHoneypot( char *local_ipv4_string )
{
	int listening_sock = initListener( local_ipv4_string, 23 );
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
	int rv = getpeername(conn_sock, (struct sockaddr *) &(remote_addr), &(remote_addr_size) );
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

	uint8_t send_buffer[] = { 0xff, 0xfc, 0x25, 0xff, 0xfc, 0x26, 0xff, 0xfd, 0x18, 0xff, 0xfd, 0x20, 0xff, 0xfd,  
	                          0x23, 0xff, 0xfd, 0x27, 0xff, 0xfd, 0x24 };
	ssize_t bytes_sent = send( conn_sock, send_buffer, (ssize_t) sizeof(send_buffer), 0);
	if (bytes_sent <= 0)
	{
		printf("error in sending bytes\n");
		return 1;
	}
	printf("sent header bytes\n");

	uint8_t recv_buffer[30];
	ssize_t bytes_received = recv( conn_sock, recv_buffer, (ssize_t) sizeof(recv_buffer), 0);
	if (bytes_received <= 0)
	{
		printf("error in receiving bytes\n");
		return 1;
	}
	printf("received bytes\n");

	return 0;
}
