#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include "main.h"

#define MAX_LEN 41
#define FREE(x) do{free(x); x=NULL;}while(0);
#define ALL_ELE(x) x->name,x->group,x->contact_num,x->mail_addr

struct _lkd
{
	char name[MAX_LEN], group[MAX_LEN], contact_num[17], mail_addr[MAX_LEN];
	struct _lkd* next;
	struct _lkd* grnext;
};
typedef struct _lkd LKD, *PLKD;

PLKD CreateNew()
{
	PLKD workspace;
	workspace = (PLKD)malloc(sizeof(struct _lkd));
	workspace->next = NULL;
	return workspace;
}


PLKD addOne(PLKD head,PLKD newone)
//Return head -> 맨 처음 값이 그대로일 때
//Return newone -> 맨 처음 값이 새로 넣은 값으로 교체될 때
{
	PLKD goingdeep;
	goingdeep = head;
	if (NULL == head)  //비었을 때
	{
		return newone;
	}
	else
	{
		while (NULL != goingdeep->next) goingdeep = goingdeep->next;
		goingdeep->next = newone; //새로 들어온 값이 마지막
		return head;
	}
}

PLKD routine_insertNewData(PLKD new)
{
	printf("이름 : "); scanf_s("%s", new->name, sizeof(new->name));
	printf("그룹 : "); scanf_s("%s", new->group, sizeof(new->group));
	printf("전화번호 : "); scanf_s("%s", new->contact_num, sizeof(new->contact_num));
	printf("메일주소 : "); scanf_s("%s", new->mail_addr, sizeof(new->mail_addr));
	system("cls");
	printf("입력 내용 확인\n이름\t그룹\t전화번호\t메일주소\n");
	return new;
}

PLKD routine_add(PLKD head) //RETURN: 링크드 리스트의 Head 노드
{
	int ck;
	fflush(stdin);
	PLKD newone = CreateNew();
	newone = routine_insertNewData(newone);
	printf("%s\t%s\t%s\t%s", newone->name, newone->group, newone->contact_num, newone->mail_addr);
	printf("\n1.입력 2.취소"); scanf_s("%d", &ck);
	if (ck != 1) 
	{
		printf("입력 취소");
		FREE(newone);
		return head;
	}
	head = addOne(head, newone);
	return head;
}


int routine_selectscan(PLKD head,char select[MAX_LEN]) //RETURN: 검색된 노드 수
{
	PLKD goingdeep = head;
	if (NULL == goingdeep) {
		printf("비어 있음\n");
		return 0;
	}
	int count = 0;
	do
	{
		printf("No\tName\tGroup\tPhone Number\tE-Mail Address\n");
		if (strcmp(goingdeep->name, select)==0)
		{
			printf("%d\t%s\t%s\t%s\t%s\n", ++count, ALL_ELE(goingdeep));
		}
		goingdeep = goingdeep->next;
	} while (NULL!= goingdeep->next);
	if (count == 0)
	{
		system("cls");
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
	if (NULL== goingdeep) {
		printf("비어 있음\n");
		return 1;
	}
	int count = 0;
	printf("No\tName\tGroup\tPhone Number\tE-Mail Address\n");
	do
	{
		printf("%d\t%s\t%s\t%s\t%s\n", ++count, ALL_ELE(goingdeep));
		goingdeep = goingdeep->next;
	} while (NULL!= goingdeep);
	printf("총 연락처 수: %d\n\n", count);
	return 0;
}

PLKD routine_deleteNode(PLKD head, char target[MAX_LEN])
{
	PLKD goingdeep,back = NULL;
	goingdeep = head;

	if (NULL==head)  //비었을 때
	{
		return NULL;
	}
	do
	{
		if (strcmp(target, goingdeep->name) == 0) //찾은 값이 삭제 대상
		{
				if (NULL == back) //삭제 대상이 처음 노드
				{
					printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(goingdeep));
					head = goingdeep->next;
					FREE(goingdeep);
					return head;
				}
				else //삭제 대상이 중간 노드
				{
					printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(goingdeep));
					back->next = goingdeep->next;
					FREE(goingdeep);
					return head;
				}
				if (NULL == goingdeep->next) //삭제 대상이 마지막 노드
				{
					printf("%s\t%s\t%s\t%s를 삭제\n", ALL_ELE(goingdeep));
					FREE(goingdeep);
					back->next = NULL;
					return head;
				}
		}
		back = goingdeep;
		if (goingdeep->next == NULL) break;
		goingdeep = goingdeep->next;
	} while (goingdeep!=NULL);
	printf("삭제 대상이 없습니다.\n");

	return head;
}

int main()
{
	PLKD head = NULL, tmp;
	char input=0,str[MAX_LEN];
	while (1)
	{
		scanf_s("%*c");
		printf("1. 연락처 추가\n2. 전체 검색\n3. 이름 검색\n4. 직장별 검색\n5. 삭제\nq. 나가기\n");
		input = getchar();
		system("cls");
		switch (input) {
		case '1':
			head = routine_add(head);
			break;
		case '2':
			routine_fullscan(head);
			break;
		case '3':
			printf("검색할 이름 입력 :");
			scanf_s("%s", str, sizeof(str));
			routine_selectscan(head,str);
			break;
		case '4': // 이거땜에 내가 Static 문자열 찾음
			routine_fullscan(head);
			break;
		case '5':
			printf("삭제할 이름 입력 :");
			scanf_s("%s", str, sizeof(str));
			head=routine_deleteNode(head,str);
			break;
		case 'q':
		case 'Q':
			return 0;
		default:
			printf("Err\n\n");
			break;
		}
		fflush(stdin);
	}
	
	while (NULL==head->next)
	{
		tmp = head;
		head->next;
		FREE(tmp);
	}
	FREE(head);
	return 0;
}