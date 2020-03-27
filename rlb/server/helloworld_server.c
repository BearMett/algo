#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include"../lkdsoc.h"


//typedef int (*func_ptr)(PLKD, *char, *char);	

int main(int argc, char **argv)
{
	const int option=1; //소켓 옵션용
	int serv_sock; //서버 소켓
	int clnt_sock; //클라이언트 소켓
	struct sockaddr_in serv_addr; // 서버 주소/프로토콜
	struct sockaddr_in clnt_addr; // 클라이언트 주소/프로토콜
	int clnt_addr_size; //클라이언트 주소 
    int input_num=0, breaker = 0, len_sock; 
	//input_num - 입력받은 숫자,breaker - 접속 해제 요청 처리용 변수, len - 전송, 수신한 데이터의 양
	char message[BUFSIZE]="\n"; //실질적으로 메시지를 작성하는 버퍼
	char itoa[9]; //정수형 문자열을 전환할 때 사용하기 위한 배열
	char target[MAX_LEN]; //다른곳과 상호작용을 위한 배열
	char intro[] = "\n\n\
1. 리스트 상태 보기\n\
2. 리스트의 이름 변경\n\
3. 리스트에 새 노드 삽입\n\
4. 리스트에 모든 노드 탐색\n\
5. 리스트에 노드 삭제\n\
-1. 나가기\n";
	//func_ptr func_ = routine_selectscan;
	
	PLMAN lists = (PLMAN)calloc(1,sizeof(struct _lkd_man));
	lists->length = 0;
	strcpy(lists->idname, "이름없음");
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

		send_buf = make_header(intro,sizeof(intro));
		len_sock = write(clnt_sock, send_buf, strlen(intro)+sizeof(int)+sizeof(char)); //데이터 전송
		//Welcome Message

		while(breaker != 1)
		{
			memset(message,0,sizeof(message));
			printf("응답 받기 시도\n");
			len_sock = rcv_msg(&clnt_sock);
			input_num = atoi(recv_buf);
			printf("%d\n",input_num);

			switch (input_num) {
				case 1://전체 리스트 상태 보기
					memcpy(message,MEM_STRCPY("리스트 이름\t리스트 노드 수 \n"));

					snprintf(message+strlen(message),SNP_SIZE(lists->idname),"%s", lists->idname);
					sprintf(itoa,"\t%d\n",lists->length);
					
					if (lists->length == 0) memcpy(message+strlen(message),MEM_STRCPY("\t비어있음\n"));
					else snprintf(message+strlen(message),SNP_SIZE(itoa),"%s", itoa);
					break;

				case 2://작업 대상 리스트의 이름 변경
					wnr("변경할 이름 입력 : ",&clnt_sock,lists->idname);
					strcpy(message,"이름 변경 완료\n");
					break;
				
				case 3://작업 대상 리스트에 새 노드 삽입
					lists = routine_add(lists,message,&clnt_sock); //함수의 매개변수로 버퍼가 돌아다닐수 있도록 해야함
					break;
				
				case 4://작업 대상 리스트에 모든 노드 탐색
					routine_fullscan(lists->head,NULL,message);
					//func_(lists->head, NULL, message);
					break;
				
				case 5://작업 대상 리스트에 노드 삭제
					memcpy(message,MEM_STRCPY("삭제할 이름 입력 : "));
					
					send_buf = make_header(message,strlen(message));	
					len_sock = write(clnt_sock, send_buf, strlen(message)+sizeof(int)+1); //데이터 전송
					printf("%d Bytes sent\n",len_sock);
					
					len_sock = rcv_msg(&clnt_sock);
					
					memset(recv_buf+len_sock,0,sizeof(char));
					memset(message,0,sizeof(char)*BUFSIZE);
					strcpy(target, recv_buf);
					lists = routine_deleteNode(lists, target, message);
					break;
				
				case -1:
					printf("걸림");
					breaker=1;//나가기
					break;

				default:
					printf("err\n");
			}
			//Answer 발신
			printf("브레이커 만남 %d\n",breaker);
			if(breaker == 1) break;
			printf("답변 받기 시도\n");
			snprintf(message+strlen(message),SNP_SIZE(intro),"%s",intro);
			send_buf = make_header(message,strlen(message));	
			printf("%ld Bytes sent\n",write(clnt_sock, send_buf, strlen(message)+sizeof(int)+1)); //데이터 전송
			printf("5: %s\n",message);
		}
		breaker =0;
	}
	printf("Free this!");
	lists->curr = lists->head;
	while(lists->curr->next != NULL)
	{
		lists->curr = lists->curr->next;
		FREE(lists->head);
	}
	
	return 0;
}

