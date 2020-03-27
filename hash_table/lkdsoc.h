#define MAX_LEN 41 //Element's Text Size
#define FREE(x) do{free(x); x=NULL;}while(0); // Free x (Atomic)
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

PLMAN routine_add(PLMAN list);//노드를 생성하고 입력받는 절차. (대상, 통신버퍼, 소켓)
PLMAN routine_deleteNode(PLMAN list, char target[MAX_LEN]);//list에서 target을 찾아 삭제, msg는 통신을 위한 버퍼

int routine_fullscan  (PLKD head, char select[MAX_LEN]);  //head(리스트)를 찾아 전체 검색, msg는 통신을 위한 버퍼
int routine_selectscan(PLKD head, char select[MAX_LEN]); //노드 검색 RETURN: 검색된 노드 수

PLKD routine_insertNewData(PLKD new); //노드의 속성 입력 (새로 만든 단일 노드)
PLKD CreateNode(); //빈 노드 생성 및 주소 반환
PLKD addNode(PLMAN list, PLKD newone); // list 에 newone 추가

void error_handling(char *message);

void dumpreal(char *target, int bytes); //덤프

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

PLKD routine_insertNewData(PLKD new) //별도 호출해야함----------------------------------------------------------------------
{
	scanf("%*c");
	printf("\nStarting Insert New Data\n");
	printf("이름 : \n");
	scanf("%s", new->name);

	printf("그룹 : \n");
	scanf("%s", new->group);

	printf("전화번호 : \n");
	scanf("%s", new->contact_num);

	printf("메일주소 : \n");
	scanf("%s", new->mail_addr);

	printf("입력 내용 확인\n이름\t그룹\t전화번호\t메일주소\n");
	return new;
}

PLMAN routine_add(PLMAN list) //RETURN: 링크를 직접 수정하는방식으로 변경
{
	int ck;
	PLKD newone;
	newone = CreateNode(); //이걸 바깥에서 하도록 바꿔야함----------------------------------------------------------------
	newone = routine_insertNewData(newone);

	list->curr = list->head;

	if(NULL == list->curr) printf("비엇음");
	while (list->curr != NULL)
	{
		if (strcmp(newone->name, list->curr->name) == 0) //찾은 값이 대상
		{
			printf("\n중복된 이름이 있습니다. 작업을 취소합니다.\n"); // 에러값 반환으로 변경 --------------------------------------
			FREE(newone);
			return list;
		}
		list->curr = list->curr->next;
	}
	printf("%s\t%s\t%s\t%s", ALL_ELE(newone));
	printf("\n1.입력 2.취소\n"); // 이거 혹시 검색하는 단계를 나누어서 앞에 먼저 하는식으로 바꾸고 이 함수는 정말 삽입만 하도록 변경 ---------------------------------------------
	scanf("%d", &ck);
	if (ck != 1)
	{
		printf("\n입력 취소");
		FREE(newone);
		return list;
	}
	
	list->head = addNode(list, newone);
	list->length = list->length + 1;
	
	return list;
}

int routine_selectscan(PLKD head, char select[MAX_LEN]) //RETURN: 검색된 노드 수 // 선택 검색과 전체 검색을 합치고 NULL을 반환할 때 까지 지속적으로 반환하는 함수로 바꾸면? --------------------
{
	PLKD goingdeep = head;
	if (NULL == goingdeep) 
	{
		printf("비어 있음\n");
		return 0;
	}
	int count = 0;
	do
	{
		if (strcmp(goingdeep->name, select) == 0)
		{
			printf("%d\t%s\t%s\t%s\t%s\n", ++count, ALL_ELE(goingdeep));
		}
		goingdeep = goingdeep->next;
	} while (NULL != goingdeep);
	if (count == 0)
	{
		printf("일치하는 결과가 없습니다\n");
	}
	else 
	{
		printf("총 연락처 수: %d\n\n", count);
	}
	return count;
}

int routine_fullscan(PLKD head, char select[MAX_LEN])  //유지대상
{
	int count = 0;
	PLKD goingdeep = head;
	if (NULL == goingdeep) 
	{
		printf("비어 있음\n");
		return 0;
	}
	printf("No\tName\tGroup\tPhone Number\t\tE-Mail Address\n");
	do
	{
		printf("%d\t%s\t%s\t%s\t%8s\n", ++count, ALL_ELE(goingdeep));
		goingdeep = goingdeep->next;
	} while (NULL != goingdeep);
	printf("총 연락처 수: %d\n\n", count);
	return count;
}

PLMAN routine_deleteNode(PLMAN list, char target[MAX_LEN])
{
	list->curr = list->head;

	if (NULL == list->head)  //비었을 때
	{
		printf("삭제 대상이 없습니다.\n");
		return list;
	}
	do
	{
		if (strcmp(target, list->curr->name) == 0) //찾은 값이 삭제 대상
		{
			list->length = list->length - 1;
			printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(list->curr));
			dumpreal((char*)list,sizeof(LMAN));
			list->curr = NULL;
			printf("삭제");
			if (NULL == list->curr->prev) //삭제 대상이 처음 노드
			{
				printf("1번상태");
				list->head = list->head->next;
				if(list->length == 0) list->head = NULL;
				else list->head->prev = NULL;
			}
			else if (NULL == list->curr->next) //삭제 대상이 마지막 노드
			{
				printf("2번상태");
				list->curr->prev->next = NULL;
			}
			else //삭제 대상이 중간 노드
			{
				printf("3번상태");
				list->curr->prev->next = list->curr->next;
				list->curr->next->prev = list->curr->prev;
			}
			FREE(list->curr);
			return list;
		}
		if (list->curr->next == NULL) break;
		list->curr = list->curr->next;
	} while (list->curr != NULL);
	printf("삭제 대상이 없습니다.\n");
	
	return list;
}

