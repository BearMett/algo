#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

#define MAX_LEN 41
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
typedef struct _lkd_man LMAN, *PLMAN;

PLKD CreateNode()
{
	PLKD workspace;
	workspace = (PLKD)calloc(1,sizeof(struct _lkd));
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
		return newone;
	}
	else
	{
		while (NULL != list->curr->next)
		{
			list->curr->next->prev = list->curr;
			list->curr = list->curr->next;
		}
		list->curr->next = newone; //새로 들어온 값이 마지막
		return list->head;
	}
}

PLKD routine_insertNewData(PLKD new) //유지대상
{
	printf("이름 : "); scanf("%s", new->name);
	printf("그룹 : "); scanf("%s", new->group);
	printf("전화번호 : "); scanf("%s", new->contact_num);
	printf("메일주소 : "); scanf("%s", new->mail_addr);
	system("clear");
	printf("입력 내용 확인\n이름\t그룹\t전화번호\t메일주소\n");
	return new;
}

PLMAN routine_add(PLMAN list) //RETURN: 링크를 직접 수정하는방식으로 변경
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
		return list;
	}
	list->head = addNode(list, newone);
	list->length = list->length + 1;
	return list;
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

int routine_fullscan(PLKD head)  //유지대상
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

PLMAN routine_deleteNode(PLMAN list, char target[MAX_LEN])
{
	list->curr = list->head;

	if (NULL == list->head)  //비었을 때
	{
		return NULL;
	}
	do
	{
		if (strcmp(target, list->curr->name) == 0) //찾은 값이 삭제 대상
		{
			if (NULL == list->curr->prev) //삭제 대상이 처음 노드
			{
				list->length = list->length - 1;
				printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(list->curr));
				list->head = list->curr->next;
				FREE(list->curr);
				return list;
			}
			else //삭제 대상이 중간 노드
			{
				list->length = list->length - 1;
				printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(list->curr));
				list->curr->prev->next = list->curr->next;
				FREE(list->curr);
				return list;
			}
			if (NULL == list->curr->next) //삭제 대상이 마지막 노드
			{
				list->length = list->length - 1;
				printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(list->curr));
				FREE(list->curr);
				list->curr->prev->next = NULL;
				return list;
			}
		}
		list->curr->prev = list->curr;
		if (list->curr->next == NULL) break;
		list->curr = list->curr->next;
	} while (list->curr != NULL);
	printf("삭제 대상이 없습니다.\n");
	return list;
}


int main()
{
	int prv_err = 0,input_num;
	char input,str[MAX_LEN];
	PLMAN lists = (PLMAN)calloc(1,sizeof(struct _lkd_man));
	lists->length = 0;
	strcpy(lists->idname, "이름없음");
	while (prv_err++ < 500)
	{
		scanf("%*c");
		printf("\n\
1. 리스트 상태 보기\n\
2. 리스트의 이름 변경\n\
3. 리스트에 새 노드 삽입\n\
4. 리스트에 모든 노드 탐색\n\
5. 리스트에 노드 삭제\n\
q. 나가기\n");
		input = getchar();
		system("cls");
		switch (input) {
		case '1'://전체 리스트 상태 보기
			printf("리스트 이름\t리스트 노드 수\n");
			printf("%s\t", lists->idname);
			if (lists->length == 0) printf("비어있음\n");
			else printf("%d", lists->length);
			break;
		case '2'://작업 대상 리스트의 이름 변경
			printf("변경할 이름 입력 : ");
			scanf("%s", str);
			strcpy(lists->idname, str);
			break;
		case '3'://작업 대상 리스트에 새 노드 삽입
			lists = routine_add(lists);
			break;
		case '4'://작업 대상 리스트에 모든 노드 탐색
			routine_fullscan(lists->head);
			break;
		case '5'://작업 대상 리스트에 노드 삭제
			printf("삭제할 이름 입력 :");
			scanf_s("%s", str, sizeof(str));
			lists = routine_deleteNode(lists, str);
			break;
		case 'q':
		case 'Q':// 나가기
			return 0;
		default:
			printf("err\n");
		}
	}
	PLKD tmp;
	while (NULL != lists->head)
	{
		tmp = lists->head;
		lists->head = lists->head->next;
		FREE(tmp);
	}
}