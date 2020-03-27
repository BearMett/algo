#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"../lkdsoc.h"
#define BUFSIZE 4096

int main(int argc, char **argv)
{
	int sock,len_sock;
	struct sockaddr_in serv_addr;
	char message[BUFSIZE]={0,};
	if(argc!=3)
	{
		printf("Usage :%s <IP> <port>\n",argv[0]);
		exit(1);
	}
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) // 서버로 연결 요청
		error_handling("connect() error!");

	while(1)
	{
		len_sock = rcv_msg(&sock);
		write(1,recv_buf,len_sock);
	 	FREE(recv_buf);	
		memset(message,0,sizeof(message));
		read(0,message,BUFSIZE);
		memset(message+strlen(message)-1,0,sizeof(char));
			
		send_buf=make_header(message,strlen(message));
		len_sock = write(sock, send_buf, strlen(message)+sizeof(int)+1);
		FREE(send_buf);
		if(atoi(message) == -1) break;
	
	}
	close(sock);
	return 0;
	
}

