#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/un.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<netinet/if_ether.h>
#include<arpa/inet.h>
#include"hash.h"
#define MAX_CLI 65536


int main()
{
	int sfd, slen, clen, ret;
	struct sockaddr_in saddr, caddr;
	char buff[100];
	struct ethhdr *eth =(struct ethhdr*)malloc(sizeof(struct ethhdr));
	table **hashtable=createTable(MAX_CLI);
	sfd= socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	clen=sizeof(caddr);
	while(1)
	{
		ret=recvfrom(sfd, buff, sizeof(buff), 0, (struct sockaddr*)&caddr, &clen);
		if(ret==-1)
		{
			perror("recvfrom");
			return -1;
		}
		param *packet=(param*)malloc(sizeof(param));
		eth = (struct ethhdr*)buff;
		packet->ip=(struct iphdr*)(buff+sizeof(struct ethhdr));
		int  iphdrlen = packet->ip->ihl*4;
		
		packet->tcp=(struct tcphdr*)(buff+iphdrlen+sizeof(struct ethhdr));
		packet->udp=(struct udphdr*)(buff+iphdrlen+(sizeof(struct ethhdr)));
		printf("Inserting packet with protocol: %d\n", packet->ip->protocol);
		insertPacket(packet, hashtable, 0, 1);
	
	}


}
