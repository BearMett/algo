
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

#define BUFSIZE 1024

void error_handling(char *message);


int init_sock(int *serv_sock,int port,struct sockaddr_in *serv_addr)
{
	*serv_sock=socket(PF_INET, SOCK_STREAM, 0); //서버 소켓 생성
	if(*serv_sock == -1) return 1;
//		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr->sin_family=AF_INET;
	serv_addr->sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr->sin_port=htons(port);
	
	return 0;
}


int bind_sock(int *serv_sock,struct sockaddr_in *serv_addr)
{
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1) return 1;//소켓에 주소 할당
	return 0;
//		error_handling("bind() error");
}

int listen_sock(int *serv_sock,int wait_num)
{
	if(listen(serv_sock, wait_num)==-1) return 1;//연결 요청 대기 상태로 진입
	return 0;
//		error_handling("listen() error");
}

int accept_sock(int *clnt_sock,int *serv_sock,struct sockaddr_in *clnt_addr,int *clnt_addr_size)
{
	clnt_sock=accept(serv_sock, (struct sockaddr*)clnt_addr, clnt_addr_size); //연결 요청 수락

	if(clnt_sock==-1) return 1;
	clnt_addr_size=sizeof(clnt_addr);
	return 0;
//		error_handling("accept() error");
}

void close_sock(int clnt_sock)
{
	close(clnt_sock);
}

int main(int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char message[BUFSIZE];

	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	if(init_sock(&serv_sock,atoi(argv[1]),*serv_addr)) return 1;
	if(bind_sock(&serv_sock,&serv_addr)) return 1;
	if(listen_sock(&serv_sock,5)) return 1;
	if(accept_sock(&clnt_sock,&serv_sock,&clnt_addr,&clnt_addr_size)) return 1;
	write(clnt_sock, message, sizeof(message)); //데이터 전송
	close_sock(clnt_sock); //연결 종료
	return 0;
}
/*
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}*/

