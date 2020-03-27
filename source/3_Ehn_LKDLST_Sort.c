#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_LEN 22
struct _lkd
{
	int cat;
	struct _lkd* next;
};

typedef struct _lkd* lkd;
typedef struct _lkd LKD, *PLKD;


lkd CreateNew()
{
	lkd workspace;
	workspace = (lkd)malloc(sizeof(struct _lkd));
	workspace->next = NULL;
	return workspace;
}

lkd addOne(lkd head, int cat)
{
	lkd newone, goingdeep, back = NULL;
	newone = CreateNew();
	newone->cat = cat;
	goingdeep = head;
	//Return head -> 맨 처음 값이 그대로일 때
	//Return newone -> 맨 처음 값이 새로 넣은 값으로 교체될 때
	
	if (head == NULL)  //비었을 때
	{
		return newone;
	}
	else while(1)
	{
		if (newone->cat <= goingdeep->cat) //다음 값이 있고 지금 검사하는 값보다 새로 넣은게 작으면
		{
			newone->next = goingdeep;
			if (back == NULL)
			{
				newone->next = goingdeep;
				return newone;
			}
			else
			{
				back->next = newone; //Replace Link			
				return head;
			}
		}
		if (goingdeep->next == NULL) //다음 값이 없으면
		{
			goingdeep->next = newone; //새로 들어온 값이 마지막
			return head;
		}
		back = goingdeep;
		goingdeep = goingdeep->next; //이전 노드 저장 후 다음 노드 확인
	}
}
/*
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
	int cat;
	fflush(stdin);
	printf("코드");
	scanf("%d", &cat);
	head = addOne(head, cat);
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
		head = head->next;
	} while (head != NULL);
	printf("총 링크 %d개 중 ", count);
	int tgt = rand() % count;
	printf("%d\n", tgt+1);
	for (i = 0; i < tgt; i++)
	{
		start = start->next;
	}
	printf("No\tCat\n");
	//printf("%d\t%s\n", tgt+1, cat_res(start->cat));
	//printf("%d\t%d\n",start->cat);
	return 0;
}
*/
int routine_fullscan(lkd head) 
{
	if (head==NULL) {
		printf("비어 있음\n");
		return 1;
	}
	int count = 0;
	printf("No\tCat\n");
	do
	{
		//printf("%d\t%s\n",++count,cat_res(head->cat));
		printf("%d\t%d\n", ++count, head->cat);
		head = head->next;
	} while (head!=NULL);
	printf("총 링크 수: %d\n\n", count);
	return 0;
}

int main()
{
	lkd head=NULL;
	char input=0,c,str[10];
	while (1)
	{
		int cat;
		printf("숫자 입력 (X to exit) : ");
		scanf("%s", str);
		if (str[0] == 'X' || str[0] == 'x') break;
		cat = atoi(str);
		head = addOne(head, cat);

		routine_fullscan(head);
		/*printf("\n1. 값 추가\n2. 무작위 뽑기\n3. 전체검색\nq. 나가기\n");
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
		fflush(stdin);*/
	}
	return 0;
}
