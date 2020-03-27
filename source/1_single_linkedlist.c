#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_LEN 22
struct _lkd
{
	char bname[MAX_LEN];
	int cat;
	struct _lkd* tail;
};

typedef struct _lkd* lkd;


lkd CreateNew()
{
	lkd workspace;
	workspace = (lkd)malloc(sizeof(struct _lkd));
	workspace->tail = NULL;
	return workspace;
}

lkd addOne(lkd head, char bname[MAX_LEN], int cat)
{
	lkd newone, goingdeep;
	newone = CreateNew();
	//newone->bname[MAX_LEN] = bname[MAX_LEN];
	strcpy(newone->bname, bname);
	newone->cat = cat;

	if (head == NULL)
	{
		head = newone;
	}
	else
	{
		goingdeep = head;
		while (goingdeep->tail != NULL) {
			goingdeep = goingdeep->tail;
		}
		goingdeep->tail = newone;
	}
	printf("이름: %s 코드: %d 입력 완료\n\n", newone->bname, newone->cat);
	return head;
}


const char* cat_res(int cate)
{
	switch (cate)
	{
	case 1:
		return "기술";
	case 2:
		return "문학";
	default:
		return "미분류";
	}
}

lkd routine_add(lkd head)
{
	char bname[MAX_LEN];
	int cat;
	fflush(stdin);
	printf("제목 코드");
	scanf("%s %d", bname,&cat);
	head = addOne(head, bname, cat);
	return head;
}

int routine_random(lkd head)
{
	lkd start=head;
	srand((unsigned)time(NULL));
	if (head == NULL) {
		printf("비어 있음\n");
		return 1;
	}
	int count = 0,i;
	do
	{
		count++;
		head = head->tail;
	} while (head != NULL);
	printf("총 링크 %d개 중 ", count);
	int tgt = rand() % count;
	printf("%d\n", tgt+1);
	for (i = 0; i < tgt; i++)
	{
		start = start->tail;
	}
	printf("No\tName\tCat\n");
	printf("%d\t%s\t%s\n", tgt+1, start->bname, cat_res(start->cat));
	return 0;
}

int routine_fullscan(lkd head) 
{
	if (head==NULL) {
		printf("비어 있음\n");
		return 1;
	}
	int count = 0;
	printf("No\tName\tCat\n");
	do
	{
		printf("%d\t%s\t%s\n",++count,head->bname,cat_res(head->cat));
		head = head->tail;
	} while (head!=NULL);
	printf("총 링크 수: %d\n\n", count);
	return 0;
}

int main()
{
	lkd head=NULL;
	char input=0,c;
	while (1)
	{
		scanf("%*c", &c);
		printf("1. 값 추가\n2. 무작위 뽑기\n3. 전체검색\nq. 나가기\n");
		input=getchar();
		switch (input) {
		case '1':
			head = routine_add(head);
			break;
		case '2':
			routine_random(head);
			break;
		case '3':
			routine_fullscan(head);
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
	return 0;
}
