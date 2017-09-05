#include "hash.h"

table** createTable(int maxsize)
{
	int i;
	table **hashtable=(table**)malloc(sizeof(table*)*4);
	for(i=0;i<4;i++)
	{
		*(hashtable+i)=(table*)malloc(sizeof(table)*MAXSIZE);
	}
	return hashtable;
}

void calcIndex(param *packet, int *key)
{

	int srcaddr=packet->ip->saddr;
	int dstaddr=packet->ip->daddr;
	int srcport;
	int dstport;

	if(packet->ip->protocol==6)
	{
		srcport=packet->tcp->source;
		dstport=packet->tcp->dest;
	}
	else
	{
			srcport=packet->udp->uh_sport;
			dstport=packet->udp->uh_dport;
	}

	key[0]=((srcaddr^dstaddr^srcport^dstport)*13)%MAXSIZE;
	key[1]=((srcaddr^dstaddr^srcport^dstport)*17)%MAXSIZE;
	key[2]=((srcaddr^dstaddr^srcport^dstport)*19)%MAXSIZE;
	key[3]=((srcaddr^dstaddr^srcport^dstport)*23)%MAXSIZE;

/*	if(packet->ip->protocol==6)
	{
		 srcport=packet->tcp->source;
		 dstport=packet->tcp->dest;
	}
	else if(packet->ip->protocol==17)
	{
		srcport=packet->udp->uh_sport;
		dstport=packet->udp->uh_dport;
		printf("src: %d dest: %d\n", srcport, dstport);
	}


	key[0]=srcaddr % MAXSIZE;
	key[1]=dstaddr % MAXSIZE;
	key[2]=srcport % MAXSIZE;
	key[3]=dstport % MAXSIZE;
*/
}

void insertPacket(param *packet, table **hashtable, int i, int print)
{
	
	int key[4];
	param *pack, *rmpack;
	calcIndex(packet, key);
	if(packet->ip->protocol==6)
	{
		//printf("inside\n");
		if(hashtable[i][key[i]].tcp==NULL)
		{
			hashtable[i][key[i]].tcp=packet;
			updatePacket(packet);
			if(print==1)
				printPacket(packet);
			printf("strored in table no : %d\n", i);
			return;
		}
		else
		{
			pack=hashtable[i][key[i]].tcp;
			printf("addresses: %d-> %d\n",pack->ip->saddr, packet->ip->saddr );
			printf("addresses des: %d-> %d\n",pack->ip->daddr, packet->ip->daddr );
			printf("port no: src: %d-> %d\n",pack->tcp->th_sport, packet->tcp->th_sport);
			printf("port no: des: %d-> %d\n",pack->tcp->th_dport, packet->tcp->th_dport);
			//if((pack->ip->saddr==packet->ip->saddr) && (pack->ip->daddr==packet->ip->daddr) && (pack->tcp->th_sport==packet->tcp->th_sport)&&(pack->tcp->th_dport==packet->tcp->th_dport))
			if(calcParam(pack)==calcParam(packet))
			{

				updatePacket(pack);

				if(print==1)
					printPacket(pack);
				printf("stored in table: %d at index: %d \n", i, key[i]);
				return;
			}
			else
			{
				printf("\n\nreallocating packet:tcp\n\n\n");
				rmpack=hashtable[i][key[i]].tcp;
				if(i<3)
				insertPacket(rmpack, hashtable, i+1, print);
				else
				insertPacket(rmpack, hashtable, 0, print);
			}

		}
	}
	if(packet->ip->protocol==17)
	{

		if(hashtable[i][key[i]].udp==NULL)
		{

			hashtable[i][key[i]].udp=packet;
			updatePacket(packet);
			if(print==1)
				printPacket(packet);
			printf("strored in table no : %d\n", i);
			return;
		}
		else
		{
			pack=hashtable[i][key[i]].udp;
			//if((pack->ip->saddr==packet->ip->saddr) && (pack->ip->saddr==packet->ip->saddr) && (pack->udp->uh_sport==packet->udp->uh_sport)&&(pack->udp->uh_dport==packet->udp->uh_dport))
			if(calcParam(pack)==calcParam(packet))
			{
				updatePacket(pack);
				if(print==1)
					printPacket(pack);
				return;
			}
			else
			{
				rmpack=hashtable[i][key[i]].udp;
				if(i<3)
				insertPacket(rmpack, hashtable, i+1, print);
				else
				insertPacket(rmpack, hashtable, i+1, print);
			}
		}

	}

}

unsigned long calcParam(param *packet)
{
	if(packet->ip->protocol==6)
	{
		return(((packet->ip->saddr)^(packet->ip->daddr))&((packet->tcp->source)^(packet->tcp->dest)));
	}
	else
	{
		return(((packet->ip->saddr)^(packet->ip->daddr))&((packet->udp->uh_sport)^(packet->udp->uh_dport)));
	}
}
void updatePacket(param *packet)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	packet->timestamp=asctime(timeinfo);
	packet->count++;
}

void deletePacket(param *packet, table **hashtable)
{
	int i, key[4];
	param *pack;
	calcIndex(packet, key);
	for(i=0;i<4;i++)
	{
		if(packet->ip->protocol==6)
		{
			pack=hashtable[i][key[i]].tcp;
			//if((pack->ip->saddr==packet->ip->saddr) && (pack->ip->saddr==packet->ip->saddr) && (pack->tcp->th_sport==packet->tcp->th_sport)&&(pack->tcp->th_dport==packet->tcp->th_dport))
			if(calcParam(pack)==calcParam(packet))
			{
				memset(pack, '\0', sizeof(pack));
				free(pack);
				hashtable[i][key[i]].tcp=NULL;
				break;
			}
		}

		else
		{
			pack=hashtable[i][key[i]].udp;
			//if((pack->ip->saddr==packet->ip->saddr) && (pack->ip->saddr==packet->ip->saddr) && (pack->udp->uh_sport==packet->udp->uh_sport)&&(pack->udp->uh_dport==packet->udp->uh_dport))
			if(calcParam(pack)==calcParam(packet))
			{
				memset(pack, '\0', sizeof(pack));
				free(pack);
				hashtable[i][key[i]].udp=NULL;
				break;
			}
		}
	}

}


void searchPacket(param *packet, table **hashtable)
{
	int i, key[4];
	param *pack;
	calcIndex(packet, key);
	for(i=0;i<4;i++)
	{
		if(packet->ip->protocol==6)
		{
			pack=hashtable[i][key[i]].tcp;
			//if((pack->ip->saddr==packet->ip->saddr) && (pack->ip->saddr==packet->ip->saddr) && (pack->tcp->th_sport==packet->tcp->th_sport)&&(pack->tcp->th_dport==packet->tcp->th_dport))
			if(calcParam(pack)==calcParam(packet))
			{
				printf("Packet Found: \n");
				printPacket(packet);
				break;
			}
		}
		else
		{
			pack=hashtable[i][key[i]].udp;
			//if((pack->ip->saddr==packet->ip->saddr) && (pack->ip->saddr==packet->ip->saddr) && (pack->udp->uh_sport==packet->udp->uh_sport)&&(pack->udp->uh_dport==packet->udp->uh_dport))
			if(calcParam(pack)==calcParam(packet))
			{
				printf("Packet Found: \n");
				printPacket(packet);
				break;
			}
		}
	}
}



void printPacket(param *packet)
{
	int port;
	char ip[INET_ADDRSTRLEN];
	printf("\nThe parameters are: \n");
	struct sockaddr_in addr;
	addr.sin_addr.s_addr=packet->ip->saddr;
	printf("src : %d\n", packet->ip->saddr);
	strcpy(ip, inet_ntoa(addr.sin_addr));
	printf("1. Source Address: %s\n", ip);
	addr.sin_addr.s_addr=packet->ip->daddr;
	printf("dest : %d\n", packet->ip->daddr);
	strcpy(ip, inet_ntoa(addr.sin_addr));
	printf("2. Destination Address: %s\n", ip);
	if(packet->ip->protocol==6)
	{
		port=packet->tcp->th_sport;
		printf("3. Source Port: %d\n", port);
		port=packet->tcp->th_dport;
		printf("4. Destination Port: %d\n", port);
		printf("5. Protocol: TCP\n");
		printf("6. timestamp: %s", packet->timestamp);
		printf("7. Count: %d\n", packet->count);
	}

	else
	{
		port=packet->udp->uh_sport;
		printf("3. Source Port: %d\n", port);
		port=packet->udp->uh_dport;
		printf("4. Destination Port: %d\n", port);
		printf("5. Protocol: UDP\n");
		printf("6. timestamp: %s", packet->timestamp);
		printf("7. Count: %d\n", packet->count);
	}

}
