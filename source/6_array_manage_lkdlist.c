#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

#define MAX_LEN 41
#define LIST_LEN 10
#define FREE(x) do{free(x); x=NULL;}while(0);
#define ALL_ELE(x) x->name,x->group,x->contact_num,x->mail_addr

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
typedef struct _lkd_man LMAN;

PLKD CreateNode()
{
	PLKD workspace;
	workspace = (PLKD)calloc(1,sizeof(struct _lkd));
	return workspace;
}

PLKD addNode(PLKD head, PLKD newone)
//Return head -> 맨 처음 값이 그대로일 때
//Return newone -> 맨 처음 값이 새로 넣은 값으로 교체될 때
{
	PLKD goingdeep;
	goingdeep = head;
	if (NULL == head)  //비었을 때
	{
		newone->prev = NULL;
		return newone;
	}
	else
	{
		while (NULL != goingdeep->next)
		{
			goingdeep->next->prev = goingdeep;
			goingdeep = goingdeep->next;
		}
		goingdeep->next = newone; //새로 들어온 값이 마지막
		return head;
	}
}

PLKD routine_insertNewData(PLKD new)
{
	printf("이름 : "); scanf("%s", new->name);
	printf("그룹 : "); scanf("%s", new->group);
	printf("전화번호 : "); scanf("%s", new->contact_num);
	printf("메일주소 : "); scanf("%s", new->mail_addr);
	system("clear");
	printf("입력 내용 확인\n이름\t그룹\t전화번호\t메일주소\n");
	return new;
}

PLKD routine_add(PLKD head) //RETURN: 링크드 리스트의 Head 노드
{
	int ck;
	fflush(stdin);
	PLKD newone = CreateNode();
	newone = routine_insertNewData(newone);
	printf("%s\t%s\t%s\t%s", ALL_ELE(newone));
	printf("\n1.입력 2.취소"); scanf("%d", &ck);
	if (ck != 1)
	{
		printf("입력 취소");
		FREE(newone);
		return head;
	}
	head = addNode(head, newone);
	return head;
}

int routine_selectscan(PLKD head, char select[MAX_LEN]) //RETURN: 검색된 노드 수
{
	PLKD goingdeep = head;
	if (NULL == goingdeep) {
		printf("비어 있음\n");
		return 0;
	}
	printf("No\tName\tGroup\tPhone Number\tE-Mail Address\n");
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
		system("clear");
		printf("%s", "일치하는 결과가 없습니다\n");
	}
	else {
		printf("총 연락처 수: %d\n\n", count);
	}
	return count;
}

int routine_fullscan(PLKD head)
{
	PLKD goingdeep = head;
	if (NULL == goingdeep) {
		printf("비어 있음\n");
		return 1;
	}
	int count = 0;
	printf("No\tName\tGroup\tPhone Number\tE-Mail Address\n");
	do
	{
		printf("%d\t%s\t%s\t%s\t%s\n", ++count, ALL_ELE(goingdeep));
		goingdeep = goingdeep->next;
	} while (NULL != goingdeep);
	printf("총 연락처 수: %d\n\n", count);
	return 0;
}

PLKD routine_deleteNode(PLKD head, char target[MAX_LEN])
{

	PLKD goingdeep;
	goingdeep = head;

	if (NULL == head)  //비었을 때
	{
		return NULL;
	}
	do
	{
		if (strcmp(target, goingdeep->name) == 0) //찾은 값이 삭제 대상
		{
			if (NULL == goingdeep->prev) //삭제 대상이 처음 노드
			{
				printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(goingdeep));
				head = goingdeep->next;
				FREE(goingdeep);
				return head;
			}
			else //삭제 대상이 중간 노드
			{
				printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(goingdeep));
				goingdeep->prev->next = goingdeep->next;
				FREE(goingdeep);
				return head;
			}
			if (NULL == goingdeep->next) //삭제 대상이 마지막 노드
			{
				printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(goingdeep));
				FREE(goingdeep);
				goingdeep->prev->next = NULL;
				return head;
			}
		}
		goingdeep->prev = goingdeep;
		if (goingdeep->next == NULL) break;
		goingdeep = goingdeep->next;
	} while (goingdeep != NULL);
	printf("삭제 대상이 없습니다.\n");

	return head;
}

int main()
{
	int prv_err = 0,input_num;
	char input,str[MAX_LEN];
	LMAN lists[LIST_LEN] = { 0, };
	for (int i = 0; i < 10; i++)
	{
		strcpy(lists[i].idname, "이름없음");
	}
	LMAN* working=&lists[0];
	PLKD tmp;
	while (prv_err++ < 500)
	{
		scanf("%*c");
		printf("\
1. 작업 대상 리스트 설정\n\
2. 전체 리스트 상태 보기\n\
3. 작업 대상 리스트의 이름 변경\n\
4. 작업 대상 리스트에 새 노드 삽입\n\
5. 작업 대상 리스트에 모든 노드 탐색\n\
6. 작업 대상 리스트에 노드 삭제\n\
q. 나가기\n");
		input = getchar();
		system("cls");
		switch (input) {
		case '1'://작업 대상 리스트 설정
			printf("작업 대상으로 설정할 리스트 번호 (0-9) : ");
			scanf("%d", &input_num);
			working = &lists[input_num];
			break;
		case '2'://전체 리스트 상태 보기
			printf("번호\t리스트 이름\t리스트 노드 수\n");
			for (int i = 0; i < 10; i++)
			{
				printf("%d\t%s\t",i, lists[i].idname);
				if (lists[i].length == 0) printf("비어있음\n");
				else printf("%d", lists[i].length);
			}
			break;
		case '3'://작업 대상 리스트의 이름 변경
			printf("변경할 이름 입력 : ");
			scanf("%s", str);
			strcpy(working->idname, str);
			break;
		case '4'://작업 대상 리스트에 새 노드 삽입
			working->head = routine_add(working->head);
			break;
		case '5'://작업 대상 리스트에 모든 노드 탐색
			routine_fullscan(working->head);
			break;
		case '6'://작업 대상 리스트에 노드 삭제
			printf("삭제할 이름 입력 :");
			scanf_s("%s", str, sizeof(str));
			working->head = routine_deleteNode(working->head, str);
			break;
		case 'q':
		case 'Q':// 나가기
			return 0;
		default:
			printf("err\n");
		}

		for (int j = 0; j < 10; j++)
		{
			working = &lists[j];
			while (NULL != working->head)
			{
				tmp = working->head;
				working->head = working->head->next;
				FREE(tmp);
			}
		}
	}
}