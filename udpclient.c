#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<sys/types.h>

int main()
{
	int sfd, ret, slen;
	struct sockaddr_in saddr;
	char buff[100];
	
	sfd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd==-1)
	{
		perror("socket");
		return -1;
	}
	
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=inet_addr("192.168.131.30");
	saddr.sin_port=htons(4884);
	slen=sizeof(saddr);
	printf("Enter packet to be sent: ");
	scanf("%[^\n]s",buff);
	ret = sendto(sfd, buff, sizeof(buff), 0, (struct sockaddr*)&saddr, slen);
	if(ret==-1)
	{
		perror("sendto");
		return -1;
	}
	
}
