#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<poll.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include"hash.h"
#define MAX_CLI 65535


int main()
{
	int i, ret, stfd, sufd, slen, clen, ctfd;
	struct sockaddr_in saddr, caddr;
	char buff[100]="";
	
	table **hashtable=createTable(MAX_CLI);
	
	struct pollfd clifd[MAX_CLI];
	memset(clifd, 0, sizeof(clifd));
	int nfds=0;
	stfd=socket(AF_INET, SOCK_STREAM, 0);
	if(stfd==-1)
	{
		perror("socket: tcp");
		return -1;
	}
	
	
	saddr.sin_addr.s_addr=inet_addr("192.168.131.30");
	saddr.sin_port=htons(4884);
	saddr.sin_family=AF_INET;
	slen=sizeof(saddr);
	clen=sizeof(caddr);
	ret=bind(stfd, (struct sockaddr*)&saddr, slen);
	if(ret)
	{
		perror("bind: tcp");
		return -1;
	}
	
	ret=listen(stfd, MAX_CLI);
	if(ret)
	{
		perror("listen");
		return -1;
	}
	
	sufd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sufd==-1)
	{
		perror("socket: udp");
		return -1;
	}

	ret=bind(sufd, (struct sockaddr*)&saddr, slen);
	if(ret)
	{
		perror("bind : udp");
		return -1;
	}

	clifd[0].fd=stfd;
	clifd[0].events=POLLIN;
	nfds++;
	clifd[1].fd=sufd;
	clifd[1].events=POLLIN;
	nfds++;
	
	

	while(1)
	{
		ret=poll(clifd, nfds, -1);
		if(ret==-1)
		{
			perror("poll");
			return -1;
		}
		

		for(i=0; i<nfds; i++)
		{
			if(clifd[i].revents & POLLIN)
			{
				
				if(clifd[i].fd==sufd)
				{
					printf("New udp connection established at : ");
					ret=recvfrom(sufd, buff, sizeof(buff), 0, (struct sockaddr*)&caddr, &clen);
					if(ret==-1)
					{
						perror("udp recv: ");
					}
					printf("%s\n", inet_ntoa(caddr.sin_addr));
					printf("The received packet is: %s\n", buff);
					param *packet=(param*)malloc(sizeof(param));
					struct iphdr *ip=(struct iphdr*)malloc(sizeof(struct iphdr));
					struct udphdr *udp=(struct udphdr*)malloc(sizeof(struct udphdr));
					
					ip->protocol=17;
					ip->saddr=caddr.sin_addr.s_addr;
					ip->daddr=saddr.sin_addr.s_addr;
					
					udp->uh_sport=htons(caddr.sin_port);
					udp->uh_dport=htons(saddr.sin_port);
					packet->ip=ip;
					packet->udp=udp;
					packet->tcp=NULL;
					printf("jgfjfihfhef\n");
					insertPacket(packet, hashtable, 0, PRINT_PKT);
					
					break;
				}

				if(clifd[i].fd==stfd)
				{
					ctfd=accept(clifd[i].fd, (struct sockaddr*)&caddr, &clen);
					if(ctfd==-1)
					{
						perror("accept");
						return -1;
					}
					
					clifd[nfds].fd=ctfd;
					clifd[nfds].events=POLLIN;	
				
					printf("New connection established at ip : %s\n",inet_ntoa(caddr.sin_addr));
					read(clifd[nfds].fd, buff, sizeof(buff));
					printf("The read data is: %s\n", buff);
					//getsockname(clifd[nfds].fd, (struct sockaddr*)&caddr, &clen);	
										
					param *packet=(param*)malloc(sizeof(param));
					struct iphdr *ip=(struct iphdr*)malloc(sizeof(struct iphdr));
					struct tcphdr *tcp=(struct tcphdr*)malloc(sizeof(struct tcphdr));
					
					ip->protocol=6;
					ip->saddr=caddr.sin_addr.s_addr;
					ip->daddr=saddr.sin_addr.s_addr;
					getsockname(clifd[nfds].fd, (struct sockaddr*)&caddr, &clen );

					tcp->th_sport=htons(caddr.sin_port);
					tcp->th_dport=htons(caddr.sin_port);
					
					packet->ip=ip;
					packet->tcp=tcp;
					packet->udp=NULL;
					
					insertPacket(packet, hashtable, 0, PRINT_PKT);
					nfds++;

				}
				
				else{	
				if(read(clifd[i].fd, buff, 100)==0)
				{
					close(clifd[i].fd);
					clifd[i].events=0;
					int j;
					for(j=i; j<nfds; j++)
					{
						clifd[j]=clifd[j+1];
					}
					nfds--;
				}
				else{
				ret=recv(clifd[i].fd, buff, sizeof(buff), 0);
				if(ret==-1)
				{
					perror("read");
				}
				printf("The received data is: %s\n", buff);
			
				
				param *packet=(param*)malloc(sizeof(param));
				struct iphdr *ip=(struct iphdr*)malloc(sizeof(struct iphdr));
				struct tcphdr *tcp=(struct tcphdr*)malloc(sizeof(struct tcphdr));
				
				ip->protocol=6;
				ip->saddr=caddr.sin_addr.s_addr;
				ip->daddr=saddr.sin_addr.s_addr;
				getsockname(clifd[i].fd, (struct sockaddr*)&caddr, &clen); 	
				tcp->th_sport=htons(caddr.sin_port);
				tcp->th_dport=htons(caddr.sin_port);
				
				packet->ip=ip;
				packet->tcp=tcp;
				packet->udp=NULL;
				
				insertPacket(packet, hashtable, 0, PRINT_PKT);
				}
			}
		}
	}
}
						
		
	
}



	
	
	































