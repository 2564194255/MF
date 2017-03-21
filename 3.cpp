#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <string.h>
#define MEMSIZE (10*1024)

typedef struct LNode//用链表的存储方式来表示内存中分配的任务 
{
	char name[10];
	int len;
	int flag;
	struct LNode *next;
}LNode;

LNode *head,*rest,*temp;
char my_memory[MEMSIZE];

void show()  
{  
    temp=head;
    int i=0;  
    while(temp)
    {
    	printf("主存分配情况如下：\n");
        printf("-------- 序号[%d] ---------\n",i++);
        printf("[任务名]:%s\n",temp->name); 
        printf("[地址]:0x%p\n",temp);
        printf("[长度]:%d\n",temp->len);
        printf("[状态]:%s\n",(temp->flag? "空闲":"已使用"));
        temp=temp->next;
    }
    return;
} 

void init()//初始化内存block中的内存分配情况
{
    head=(LNode *)my_memory;
    strcpy(head->name,"OSEX");
    head->next=NULL;
    head->flag=1;
    head->len=MEMSIZE-sizeof(LNode);
    show();
}

void distribute(char newname[],int newlen)//内存分配函数，使用顺序查找的方式分配内存
{
	temp=head;
	while(temp)
	{
		if(temp->len>=newlen && temp->flag==1)
			break;
		temp=temp->next;
	}
	if(!temp || newname=="OSEX")
	{
		printf("分配失败！\n");
		return;
	}
    rest=(LNode *)((char *)(temp+1)+newlen);  
    strcpy(rest->name,"OSEX");
    rest->next=NULL;
    rest->flag=1;
    rest->len=temp->len-newlen-sizeof(LNode);
    rest->next=temp->next;
    strcpy(temp->name,newname);
    temp->flag=0;
    temp->len=newlen;
    temp->next=rest;
	printf("内存分配成功！\n");
	show();
	return;
}

void reclaim(char newname[])//内存回收函数 
{
	LNode *a,*b;
	a=head;
	temp=head; 
	while(strcmp(temp->name,newname)!=0 && temp) 
		temp=temp->next;
	if(!temp || temp->flag==1)
	{
		printf("找不到该任务，回收失败！\n");
		return;
	}
	while(a)
	{
		if(a->next==temp)
		break;
		a=a->next;
	}
	b=temp->next;
    temp->flag=1;
    strcpy(temp->name,"OSEX");
    if(b && b->flag==1)  
    {
        temp->next=b->next;  
        temp->len=temp->len+sizeof(LNode)+b->len;
    }
    if(a && a->flag==1)  
    {
        a->next=temp->next;  
        a->len=a->len+sizeof(LNode)+temp->len;
    }
	printf("内存回收成功！\n");
	show();
	return;
}

int main()
{
	int selection;
	char newname[10];
	int newlen;
	init();
	while(1)
	{
		printf(" 1 --- 分配主存\n 2 --- 回收主存\n 3 --- 退出\n");
		printf("请输入选择项：");
		scanf("%d",&selection);
		switch(selection)
		{
			case 1:
				printf("请输入任务名：");
				scanf("%s",newname);
				printf("请输入任务所需的内存字节数：");
				scanf("%d",&newlen);
				distribute(newname,newlen);
				break;
			case 2:
				printf("请输入要回收的任务名：");
				scanf("%s",newname);
				reclaim(newname);
				break;
			case 3:
				printf("程序结束！\n");
				exit(0);
			default:
				printf("无此选项！\n");
		}
	}
	return 0;
}

