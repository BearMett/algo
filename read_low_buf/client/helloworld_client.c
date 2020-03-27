	/*
	 * helloworld_client.c
	 * Written by SW. YOON
	 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define BUFSIZE 10
#define HEAD_LEN 3

void error_handling(char *message);
char* rcv_msg(int *sock);

int main(int argc, char **argv)
{
	int sock;
	struct sockaddr_in serv_addr;
	char *message;

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
	message = rcv_msg(&sock);
	close(sock);
	return 0;
	
}


void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

char* rcv_msg(int *sock)
{	
	char static making[BUFSIZE];
	char message[BUFSIZE],lench[4];
	int str_len,total_len=0;
	total_len = read(*sock, message, sizeof(message) -1); //데이터 수신
	int i,explen;
	for(i = 0; i<HEAD_LEN; i++)
	{
		lench[i]=message[i];
	}
	memset(making,0,sizeof(making));
	strcat(making,message);
	printf("lench %d has gone to NULL\n",i); //헤더로 받은 길이의 끝에 NULL 처리
	lench[i] = 0;
	explen = atoi(lench); // lench를 숫자로 변환해서 expected Length를 도출
	printf("rcvd header : %d Bytes\n",explen);
	printf("Actu Rcvd : %d Bytes\n",total_len);
	printf("Message Size : %ld\n",sizeof(message)); // 순서대로 받은 헤더, 실제 수신, 버퍼 사이즈 출력
	printf("string is : '%s'\ncopying making %d to %d\ntotal_len is : %d\n",message,0,total_len,total_len);
	while(total_len < explen)
	{
		printf("\n\n");
		str_len = read(*sock, message, sizeof(message) -1); //데이터 수신
		if(str_len==-1)
			error_handling("read error!");
		printf("Making %d has benn nulled\n Message %d has been nulled\n",total_len,str_len);
		making[total_len] = 0;
		printf("\n%s\n",making);
		message[str_len] = 0;
//		memcpy(ptr+total_len,message,sizeof(char)*str_len); //받은 메시지를 기존 메시지에 붙임
		making[total_len] = 0;
		strcat(making,message);
		total_len = total_len + str_len;
		printf("string is : '%s'\n result is : '%s'\ncopying making %d to %d\nstr len is : %d / total_len is : %d\n"\
				,message,making,total_len-str_len,total_len,str_len,total_len);
		sleep(0.3);
	}
	printf("null target %c at %d\n",message[total_len],total_len);
	making[total_len]=0;
	printf("nulled %c at %d\n",message[total_len],total_len);
	printf("\n\nMessage form server : %s\nexpected : %d\n",making,explen);
	for(int j=0;j<50;j++)
	{
		printf("%3d",j);
	}
	printf("\n");
	for(int j=0;j<50;j++)
	{
		printf("%3c",making[j]);
	}
	printf("\n");
	for(int j=50;j<100;j++)
	{
		printf("%3d",j);
	}
	printf("\n");
	for(int j=50;j<100;j++)
	{
		printf("%3c",making[j]);
	}
	printf("\n");
	return making;
}
