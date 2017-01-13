
#include "main.h"
#include "telnet_hp.h"



int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("usage: ./hp <local ip> <local port>\n");
		return 1;
	}

	char *local_ipv4_string = argv[1];
	uint16_t local_port = (uint16_t) atoi(argv[2]);

	if (local_port == 23)
	{
		runTelnetHoneypot( local_ipv4_string );
	}
	else
	{
		printf("no honeypot support for port %u\n", local_port);
		return 1;
	}
	

	return 0;
}
