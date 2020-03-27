#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include"lkdsoc.h"


enum TYPE{
	_CHAR,
	_int32_t
};

int32_t func_hash(void* keyvalue,uint32_t keylen,int arr_size);
//int32_t hash_string(char *, int32_t arr_size);
//int32_t hash_string(char *, int32_t str_len, int32_t arr_size);
PLMAN hmap_create(void* dummy, int32_t arr_size);
PLMAN hmap_resize(void* source, int32_t newarr_size);
int32_t hmap_insert(PLKD target, PLMAN hmap,int32_t arr_size);
int32_t hmap_query(PLKD target, PLMAN hmap,int32_t arr_size);

int32_t main()
{
	char name[MAX_LEN], contact_num[17];
	uint32_t arr_size=11, i;
	scanf("%s", name);
	printf("\nhash : %d\n",func_hash(name,strlen(name),arr_size));
	PLMAN hmap = hmap_create(NULL,arr_size);

	PLKD newnode = CreateNode();
	routine_insertNewData(newnode);
	hmap_insert(newnode,hmap,arr_size);
	
	PLKD searchnode = CreateNode();
	strcpy(searchnode->name,"김영민");
	hmap_query(searchnode,hmap,arr_size);
/*	for(i = 0; i < arr_size ; i++)
	{
		(hmap+(i*sizeof(LMAN)))->head = addNode((hmap+(i*sizeof(LMAN))), hmapnode);
		strcpy((hmap+(i*sizeof(LMAN)))->idname,"고고고");
		printf("아이디이름 : %s\n",(hmap+(i*sizeof(LMAN)))->idname);
		strcpy((hmap+(i*sizeof(LMAN)))->head->name,"김영민");
		strcpy((hmap+(i*sizeof(LMAN)))->head->contact_num,"01049110340");
		printf("%d 번째 이름이 %s 랑 번호는 %s\n",i,(hmap+(i*sizeof(LMAN)))->head->name,(hmap+(i*sizeof(LMAN)))->head->contact_num);
	}
	
	for(i=0; i< arr_size; i++)
	{
		dumpreal((char*)(hmap+(i*sizeof(LMAN)))->head,sizeof(LKD));
	}
	dumpreal((char*)hmap,sizeof(LMAN)*arr_size);*/

	free(newnode);
	free(searchnode);
	free(hmap);
	return 0;
}

int32_t func_hash(void* keyvalue,uint32_t keylen,int32_t arr_size)
{
	char* idx_wrkplace;
	const int32_t fold_size=7;
	uint32_t index=0,fold;

	idx_wrkplace = malloc(sizeof(char)*fold_size);
	memcpy(idx_wrkplace,(char*)keyvalue,sizeof(char)*fold_size);
	dumpreal(idx_wrkplace,fold_size);
	printf("\nstarting loop\n");
	for(fold = 0; fold < fold_size / 2; fold ++)
	{
		unsigned char op1 = *(idx_wrkplace + fold),op2 = *(idx_wrkplace + fold_size - fold-1);
		printf("\n%d(+ %d) + %d(+%d - %d) = %d\n",op1,fold,op2,fold_size,fold,op1+op2);
		*(idx_wrkplace + (fold_size / 2) - fold) = op1 + op2;
		dumpreal(idx_wrkplace,fold_size);
	}
	memcpy(&index,idx_wrkplace,sizeof(int32_t));
	printf("\norigin : %u \n",index);
	index = index % arr_size;
	free(idx_wrkplace);
	return index;
}


PLMAN hmap_create(void* dummy, int32_t arr_size)
{
	PLMAN newmap;
	if((char*)dummy == NULL)
	{
		newmap = malloc(sizeof(LMAN) * arr_size);
		memset(newmap, 0, sizeof(LMAN) * arr_size);
		dumpreal((char*)newmap,sizeof(LMAN) * arr_size);
	}
	else
	{
	}
	return newmap;
}

PLMAN hmap_resize(void* source, int32_t newarr_size)
{
	return NULL;
}

int32_t hmap_insert(PLKD target, PLMAN hmap,int32_t arr_size)
{
	uint32_t idx;
	idx = func_hash(target->name,strlen(target->name),arr_size);
	printf("\nidx %d has inserted\n",idx);
	(hmap+(idx*sizeof(LMAN)))->head = addNode( (hmap + (idx * sizeof(LMAN))), target );
	printf("\n%s\n",(hmap+(idx*sizeof(LMAN)))->head->name);
}

int32_t hmap_query(PLKD target, PLMAN hmap,int32_t arr_size)
{
	uint32_t idx;
	idx = func_hash(target->name,strlen(target->name),arr_size);
	(hmap + (idx * sizeof(PLMAN)))->curr = (hmap + (idx * sizeof(PLMAN)))->head;
	while(NULL == ((hmap + (idx * sizeof(PLMAN)))->head))		
	{
		if(strcmp(target->name, (hmap + sizeof(PLMAN))->curr->name) == 0)
		{
			printf("있네!");
			return 1;
		}

	}
	printf("없네");
	return 0;
}
















