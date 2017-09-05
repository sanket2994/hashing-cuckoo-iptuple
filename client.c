#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int cfd,sfd,ret;
	struct sockaddr_in caddr,saddr;
	socklen_t clen,slen;
	char buff[100];//="greetings from client";
	char buff1[100];
	cfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sfd<0)
	{
		perror("socket: ");
		return -1;
	}

	caddr.sin_family=AF_INET;
	caddr.sin_addr.s_addr=inet_addr("192.168.131.30");
	caddr.sin_port=htons(4884);
	clen=sizeof(caddr);
	slen=sizeof(saddr);

	

	ret=connect(cfd, (struct sockaddr *)&caddr, clen);
	if(ret==-1)
	{
		perror("connect: ");
		return -1;
	}
	printf("Enter the message to be sent: ");
	scanf("%[^\n]s",buff);
	write(cfd, buff, sizeof(buff));

}
