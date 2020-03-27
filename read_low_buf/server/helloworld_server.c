	/*
	 * helloworld_server.c
	 * Written by SW. Yoon
	 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define HEAD_LEN 3
#define BUFSIZE 1000

void error_handling(char *message);
int make_header(char *msg,int msg_len);

int main(int argc, char **argv)
{	
	const int option=1;
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char message[BUFSIZE]="thisisstartofthismessage879123456789123456789123456789123456789123456789thisisendofthismessageef\n";
	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0); //서버 소켓 생성
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)//소켓에 주소 할당
		error_handling("bind() error");
	while(1)
	{
		if(listen(serv_sock, 5)==-1) //연결 요청 대기 상태로 진입
			error_handling("listen() error");
		
		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); //연결 요청 수락
		if(clnt_sock==-1)
			error_handling("accept() error");
		make_header(message,strlen(message));	
		printf("%ld Bytes snet",write(clnt_sock, message, strlen(message))); //데이터 전송
		close(clnt_sock); //연결 종료
	}
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


int make_header(char *msg,int msg_len)
{
    char len[4] = {0};
	char wrking_str[BUFSIZE];
	msg_len = msg_len + HEAD_LEN;
    len[0]=(msg_len/100)+48;
    len[1]=(msg_len-((msg_len/100)*100))/10+48;
    len[2]=msg_len-((msg_len/10)*10)+48;
    len[3]=0;

    strcpy(wrking_str,len);
    strcat(wrking_str,msg);
	strcpy(msg,wrking_str);
    printf("\n%d %d %d\n",msg_len/100,(msg_len-((msg_len/100)*100))/10,msg_len-((msg_len/10)*10));
    printf("\nwrking_str len is : %d\nmade header is : %s\nSetting Header : %s\n",msg_len,len,msg);
    return 0;
}


