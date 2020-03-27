#define MAX_LEN 41 //Element's Text Size
#define FREE(x) do{free(x); x=NULL;}while(0); // Free x (Atomic)
#define SNP_SIZE(x) strlen(x)+sizeof(char) //Calc Size for sprintf (str + NULL)
#define SNP_SIZEP(x) strlen(x)+sizeof(char),x //Calc Size for sprintf (str + NULL) and printf
#define MEM_STRCPY(x) x,sizeof(x) //멤카피를 위한 텍스트
#define ALL_ELE(x) x->name,x->group,x->contact_num,x->mail_addr //All Elements of _lkd
#define ELE_SIZE sizeof(char)*41*3+17 //All Element's size char[41]*3 + char[17] = (140)
#define BUFSIZE 4096 //4KiB Buffer Size

struct _lkd
{
	char name[MAX_LEN], group[MAX_LEN], contact_num[17], mail_addr[MAX_LEN];
	struct _lkd* next;
	struct _lkd* prev;
};
typedef struct _lkd LKD, *PLKD;

struct _lkd_man
{
	char idname[MAX_LEN];
	int length;
	struct _lkd* head;
	struct _lkd* tail;
	struct _lkd* curr;
};
typedef struct _lkd_man LMAN, *PLMAN;

PLMAN routine_add(PLMAN list, char *message, int *sock); //노드를 생성하고 입력받는 절차. (대상, 통신버퍼, 소켓)
PLMAN routine_deleteNode(PLMAN list, char target[MAX_LEN], char *msg);//list에서 target을 찾아 삭제, msg는 통신을 위한 버퍼

int routine_fullscan  (PLKD head, char select[MAX_LEN], char *msg);  //head(리스트)를 찾아 전체 검색, msg는 통신을 위한 버퍼
int routine_selectscan(PLKD head, char select[MAX_LEN], char *msg); //노드 검색 RETURN: 검색된 노드 수

PLKD routine_insertNewData(PLKD new, int *sock, char *message); //노드의 속성 입력 (새로 만든 단일 노드, 소켓, 버퍼)
PLKD CreateNode(); //빈 노드 생성 및 주소 반환
PLKD addNode(PLMAN list, PLKD newone); // list 에 newone 추가



void error_handling(char *message);
char* make_header(char* inp_msg, int msg_len); //inp_msg를 토대로 송신용 메시지 생성(static 배열) (int+char[BUFSIZE-sizeof(int))
int rcv_msg(int *sock);//소켓에서 메시지 수신 및 해석
int wnr(char *msg, int *sock, char *out);//write and receive

void dumpreal(char *target, int bytes); //덤프


char *recv_buf=NULL,*send_buf=NULL;	//수신/발신을 위한 버퍼 지정용 전역 포인터 변수

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void dumpreal(char *target, int bytes)
{
	char *data = (char *) target;
	int ii;
	printf("DUMP: %d", bytes);
	for(ii = 0; ii < bytes; ii++)
	{
		if((ii&15) == 0) printf("\n\t");
		printf("%02X ", 0x00FF & data[ii]);
	}
	printf("\n");
}

char* make_header(char* inp_msg, int msg_len)
{
	int tonet;
	char* msg = NULL;
	
	msg = (char*) malloc(sizeof(int) + msg_len);

	if(BUFSIZE - msg_len <= sizeof(int)) printf("CAUTION! BUFFER MEMORY MAY BE INSUFFICIENT"); //sizeof int + char(NULL)

	tonet = htonl(msg_len);
	memcpy(msg, &tonet, sizeof(int));
	memcpy(msg+sizeof(int), inp_msg, strlen(inp_msg)+sizeof(char));

	memset(msg+(sizeof(int) + msg_len),0,sizeof(char));
	
	//dumpreal(msg, sizeof(int) + sizeof(char) + msg_len);
	return msg;
}

int rcv_msg(int *sock)
{	
	char* making = NULL;
	char message[BUFSIZE];
	char *p_wrk = NULL;
    int payload_len, actual_len, exp_len, header_num;

	actual_len = read(*sock, message, sizeof(message) -1); //데이터 수신

	if(actual_len < sizeof(int))
	{
		strcpy(making, "No Message From Other Side! !");
		printf("No Message From Other Side! !");
		recv_buf = making;
		return -1;
	}

	memcpy(&header_num, message, sizeof(int));
	exp_len = ntohl(header_num);

	
	making = (char*)malloc(sizeof(char)*exp_len+sizeof(char));

	//dumpreal(message, actual_len);
	p_wrk = message + sizeof(int);
	memset(making, 0, sizeof(making));
	memcpy(making, p_wrk, sizeof(char) * (actual_len - sizeof(int)));

	//printf("rcvd header : %d Bytes\n", exp_len);
	//printf("Actu Rcvd : %d Bytes\n", actual_len);


	while(actual_len < exp_len)
	{
		payload_len = read(*sock, message, sizeof(message) - sizeof(char)); //데이터 수신
		
		if(payload_len==-1)
			error_handling("read error!");

		making[actual_len] = 0;
		message[payload_len] = 0;

		snprintf(making + actual_len, payload_len + sizeof(char), "%s", message);
		actual_len = actual_len + payload_len;
		sleep(0.8);
	}

	making[actual_len]=0;
	recv_buf = making;
	return actual_len - sizeof(int);
}


PLKD CreateNode()
{
	PLKD workspace;
	workspace = (PLKD) calloc(1, sizeof(struct _lkd));
	workspace->prev = NULL;
	workspace->next = NULL;
	
	return workspace;
}

PLKD addNode(PLMAN list, PLKD newone)
//Return head -> 맨 처음 값이 그대로일 때
//Return newone -> 맨 처음 값이 새로 넣은 값으로 교체될 때
{
	list->curr = list->head;
	if (NULL == list->head)  //비었을 때
	{
		newone->prev = NULL;
		newone->next = NULL;
		return newone;
	}
	while (NULL != list->curr->next)
	{
		list->curr->next->prev = list->curr;
		list->curr = list->curr->next;
	}
	newone->prev = list->curr; //방금 조사한 노드가 마지막의 이전 노드
	list->curr->next = newone; //새로 들어온 값이 마지막
	return list->head;
}

PLKD routine_insertNewData(PLKD new, int *sock, char *msg)
{
	printf("\nStarting Insert New Data\n");
	snprintf(msg, sizeof("이름 : "),"이름 : ");
	wnr(msg,sock, new->name);

	memcpy(msg, MEM_STRCPY("그룹 : ")); 
	wnr(msg,sock, new->group);

	memcpy(msg, MEM_STRCPY("전화번호 : "));
	wnr(msg,sock, new->contact_num);

	memcpy(msg, MEM_STRCPY("메일주소 : "));
	wnr(msg,sock, new->mail_addr);

	strcpy(msg,"입력 내용 확인\n이름\t그룹\t전화번호\t메일주소\n");
	return new;
}

int wnr(char *msg, int *sock, char *out)//write and receive
{//msg 내용으로 sock에 질의, 답변을 out에 저장
	int len_sock;
	send_buf = make_header(msg, strlen(msg));	
	
	len_sock = write(*sock, send_buf, strlen(msg)+sizeof(int)+sizeof(char)); //데이터 전송
	printf("%d Bytes sent\n", len_sock);

	len_sock = rcv_msg(sock);
	//printf("받은 문자열 길이는 이것 :%d:",len_sock);
	memset(recv_buf + strlen(recv_buf), 0, sizeof(char));
	
	printf("\n-------\n%s\n------\n", recv_buf);
	snprintf(out, strlen(recv_buf) + sizeof(char), "%s", recv_buf);
	return 0;
}

PLMAN routine_add(PLMAN list, char *msg, int *sock) //RETURN: 링크를 직접 수정하는방식으로 변경
{
	int ck, len_sock;
	PLKD newone = CreateNode();
	newone = routine_insertNewData(newone, sock, msg);

	list->curr = list->head;

	if(NULL == list->curr) printf("비엇음");
	while (list->curr != NULL)
	{
		if (strcmp(newone->name, list->curr->name) == 0) //찾은 값이 대상
		{
			memcpy(msg + strlen(msg), MEM_STRCPY("\n중복된 이름이 있습니다. 작업을 취소합니다.\n"));// scanf("%d", &ck);
			FREE(newone);
			return list;
		}
		list->curr = list->curr->next;
	}
	snprintf(msg + strlen(msg), ELE_SIZE + sizeof(char)*4,"%s\t%s\t%s\t%s", ALL_ELE(newone));
	snprintf(msg + strlen(msg), sizeof("\n1.입력 2.취소\n"), "\n1.입력 2.취소\n");// scanf("%d", &ck);
	send_buf = make_header(msg, strlen(msg));	

	len_sock = write(*sock, send_buf, strlen(msg) + sizeof(int) + sizeof(char)); //데이터 전송
	memset(msg,0,sizeof(char)*BUFSIZE);

	len_sock = rcv_msg(sock);
	//printf("받은 문자열 길이는 :%d:",len_sock);
	memset(recv_buf + len_sock, 0, sizeof(char));
	
	ck = atoi(recv_buf);
	if (ck != 1)
	{
		memcpy(msg + strlen(msg), MEM_STRCPY("\n입력 취소"));
		FREE(newone);
		return list;
	}
	
	list->head = addNode(list, newone);
	list->length = list->length + 1;
	
	return list;
}

int routine_selectscan(PLKD head, char select[MAX_LEN], char *msg) //RETURN: 검색된 노드 수
{
	PLKD goingdeep = head;
	if (NULL == goingdeep) 
	{
		memcpy(msg + strlen(msg), MEM_STRCPY("비어 있음\n"));
		return 0;
	}
	int count = 0;
	do
	{
		if (strcmp(goingdeep->name, select) == 0)
		{
			snprintf(msg+strlen(msg), ELE_SIZE + sizeof(char) * 15, "%d\t%s\t%s\t%s\t%s\n", ++count, ALL_ELE(goingdeep));
		}
		goingdeep = goingdeep->next;
	} while (NULL != goingdeep);
	if (count == 0)
	{
		system("clear");
		memcpy(msg+strlen(msg), MEM_STRCPY("일치하는 결과가 없습니다\n"));
	}
	else 
	{
		snprintf(msg + strlen(msg), sizeof(char) * 9 + SNP_SIZE("총 연락처 수: aa \n\n"), "총 연락처 수: %d\n\n", count);
	}
	return count;
}

int routine_fullscan(PLKD head, char select[MAX_LEN], char* msg)  //유지대상
{
	int count = 0;
	PLKD goingdeep = head;
	if (NULL == goingdeep) 
	{
		memcpy(msg, MEM_STRCPY("비어 있음\n"));
		return 0;
	}
	memcpy(msg, MEM_STRCPY("No\tName\tGroup\tPhone Number\t\tE-Mail Address\n"));
	do
	{
		snprintf(msg + strlen(msg), sizeof(char) * 15 + ELE_SIZE, "%d\t%s\t%s\t%s\t%8s\n", ++count, ALL_ELE(goingdeep));
		goingdeep = goingdeep->next;
	} while (NULL != goingdeep);
	snprintf(msg + strlen(msg), sizeof(char) * 9 + SNP_SIZE("총 연락처 수: \n\n"), "총 연락처 수: %d\n\n", count);
	return count;
}

PLMAN routine_deleteNode(PLMAN list, char target[MAX_LEN],char *msg)
{
	list->curr = list->head;

	if (NULL == list->head)  //비었을 때
	{
		memcpy(msg + strlen(msg), MEM_STRCPY("삭제 대상이 없습니다.\n"));
		return list;
	}
	do
	{
		if (strcmp(target, list->curr->name) == 0) //찾은 값이 삭제 대상
		{
			list->length = list->length - 1;
			snprintf(msg + strlen(msg), sizeof(char) * 11 + ELE_SIZE, "%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(list->curr));
			if (NULL == list->curr->prev) //삭제 대상이 처음 노드
			{
				list->head = list->head->next;
				if(list->length == 0) list->head = NULL;
				else list->head->prev = NULL;
			}
			else if (NULL == list->curr->next) //삭제 대상이 마지막 노드
			{
				list->curr->prev->next = NULL;
			}
			else //삭제 대상이 중간 노드
			{
				list->curr->prev->next = list->curr->next;
				list->curr->next->prev = list->curr->prev;
			}
			FREE(list->curr);
			return list;
		}
		if (list->curr->next == NULL) break;
		list->curr = list->curr->next;
	} while (list->curr != NULL);
	memcpy(msg + strlen(msg), MEM_STRCPY("삭제 대상이 없습니다.\n"));
	
	return list;
}

