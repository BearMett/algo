#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include "main.h"

#define MAX_LEN 41
#define FREE(x) do{free(x); x=NULL;}while(0);
#define ALL_ELE(x) x->name,x->group,x->contact_num,x->mail_addr

struct _lkdnode
{
	char name[MAX_LEN], group[MAX_LEN], contact_num[17], mail_addr[MAX_LEN];
	struct _lkdnode* next;
	struct _lkdnode* back;
}; typedef struct _lkdnode LKDN, * PLKD_NODE;

struct _lkdman
{
	char name[MAX_LEN];
	unsigned int len;
	PLKD_NODE head, tail, cur;
}; typedef struct _lkdman, * PLKD_MAN;



PLKD_NODE CreateNode()
{
	PLKD_NODE workspace;
	workspace = (PLKD_NODE)malloc(sizeof(struct _lkdnode));
	workspace->next = NULL;
	return workspace;
}


PLKD_NODE AddNode(PLKD_NODE head, PLKD_NODE newone)
//Return head -> �� ó�� ���� �״���� ��
//Return newone -> �� ó�� ���� ���� ���� ������ ��ü�� ��
{
	PLKD_NODE goingdeep;
	goingdeep = head;
	if (NULL == head)  //����� ��
	{
		return newone;
	}
	else
	{
		while (NULL != goingdeep->next) goingdeep = goingdeep->next;
		goingdeep->next = newone; //���� ���� ���� ������
		return head;
	}
}

PLKD_NODE routine_insertNewData(PLKD_NODE new)
{
	printf("�̸� : "); scanf_s("%s", new->name, sizeof(new->name));
	printf("�׷� : "); scanf_s("%s", new->group, sizeof(new->group));
	printf("��ȭ��ȣ : "); scanf_s("%s", new->contact_num, sizeof(new->contact_num));
	printf("�����ּ� : "); scanf_s("%s", new->mail_addr, sizeof(new->mail_addr));
	system("cls");
	printf("�Է� ���� Ȯ��\n�̸�\t�׷�\t��ȭ��ȣ\t�����ּ�\n");
	printf("%s\t%s\t%s\t%s", ALL_ELE(new));
	return new;
}

PLKD_NODE routine_add(PLKD_NODE head) //RETURN: ��ũ�� ����Ʈ�� Head ���
{
	int ck;
	fflush(stdin);
	PLKD_NODE newone = CreateNew();
	newone = routine_insertNewData(newone);
	printf("\n1.�Է� 2.���\n"); scanf_s("%d", &ck);
	if (ck != 1)
	{
		printf("�Է� ���");
		FREE(newone);
		return head;
	}
	head = addOne(head, newone);
	return head;
}


int routine_selectscan(PLKD_NODE head, char select[MAX_LEN]) //RETURN: �˻��� ��� ��
{
	PLKD_NODE goingdeep = head;
	if (NULL == goingdeep) {
		printf("��� ����\n");
		return 0;
	}
	int count = 0;
	printf("No\tName\tGroup\tPhone Number\tE-Mail Address\n");
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
		system("cls");
		printf("%s", "��ġ�ϴ� ����� �����ϴ�\n");
	}
	else {
		printf("�� ����ó ��: %d\n\n", count);
	}
	return count;
}

int routine_fullscan(PLKD_NODE head)
{
	PLKD_NODE goingdeep = head;
	if (NULL == goingdeep) {
		printf("��� ����\n");
		return 1;
	}
	int count = 0;
	printf("No\tName\tGroup\tPhone Number\tE-Mail Address\n");
	do
	{
		printf("%d\t%s\t%s\t%s\t%s\n", ++count, ALL_ELE(goingdeep));
		goingdeep = goingdeep->next;
	} while (NULL != goingdeep);
	printf("�� ����ó ��: %d\n\n", count);
	return 0;
}

PLKD_NODE routine_deleteNode(PLKD_NODE head, char target[MAX_LEN])
{
	PLKD_NODE goingdeep, back = NULL;
	goingdeep = head;

	if (NULL == head)  //����� ��
	{
		return NULL;
	}
	do
	{
		if (strcmp(target, goingdeep->name) == 0) //ã�� ���� ���� ���
		{
			if (NULL == back) //���� ����� ó�� ���
			{
				printf("%s\t%s\t%s\t%s�� ����\n", ALL_ELE(goingdeep));
				head = goingdeep->next;
				FREE(goingdeep);
				return head;
			}
			else //���� ����� �߰� ���
			{
				printf("%s\t%s\t%s\t%s�� ����\n", ALL_ELE(goingdeep));
				back->next = goingdeep->next;
				FREE(goingdeep);
				return head;
			}
			if (NULL == goingdeep->next) //���� ����� ������ ���
			{
				printf("%s\t%s\t%s\t%s�� ����\n", ALL_ELE(goingdeep));
				FREE(goingdeep);
				back->next = NULL;
				return head;
			}
		}
		back = goingdeep;
		if (goingdeep->next == NULL) break;
		goingdeep = goingdeep->next;
	} while (goingdeep != NULL);
	printf("���� ����� �����ϴ�.\n");

	return head;
}

int main()
{
	PLKD_NODE head = NULL, tmp;
	char input = 0, str[MAX_LEN];
	while (1)
	{
		scanf_s("%*c");
		//printf("1. ����ó �߰�\n2. ��ü �˻�\n3. �̸� �˻�\n5. ����\nq. ������\n");
		printf("1. ����ó �߰�\n2. ��ü �˻�\n3. �̸� �˻�\n4. ���庰 �˻�\n5. ����\nq. ������\n");
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
			printf("�˻��� �̸� �Է� :");
			scanf_s("%s", str, sizeof(str));
			routine_selectscan(head, str);
			break;
			/*case '4': // �̰Ŷ��� ���� Static ���ڿ� ã��
				routine_fullscan(head);
				break;*/
		case '5':
			printf("������ �̸� �Է� :");
			scanf_s("%s", str, sizeof(str));
			head = routine_deleteNode(head, str);
			break;
		case 'q':
		case 'Q':
			return 0;
		default:
			printf("Err\n\n");
			break;
		}
	}

	while (NULL == head->next)
	{
		tmp = head;
		head->next;
		FREE(tmp);
	}
	FREE(head);
	return 0;
}