#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#ifndef MAXSIZE
#define MAXSIZE 65535
#endif
#define PRINT_PKT 1
typedef struct {
	struct iphdr *ip;
	struct tcphdr *tcp;
	struct udphdr *udp;
	int count;
	char *timestamp;
	}param;

typedef struct {
	 param *tcp;
	 param *udp;
}table;
table** createTable(int max);
void calcIndex(param *packet, int *key);
void insertPacket(param *packet, table **hashtable, int i, int print);
void deletePacket(param *packet, table **hashtable);
void searchPacketUDP(param *packet, table **hashtable);
void printPacket(param *packet);
void updatePacket(param *packet);
unsigned long calcParam(param *packet);
