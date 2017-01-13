#ifndef _SOCKET_FUNC_H
#define _SOCKET_FUNC_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IPV4_LENGTH 16		// buffer overflow risk when using inet_ntop, etc? look into this later


int initListener( char *_local_ipv4_string, uint16_t _local_port );

#endif
