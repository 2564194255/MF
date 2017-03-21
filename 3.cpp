#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <string.h>
#define MEMSIZE (10*1024)

typedef struct LNode//������Ĵ洢��ʽ����ʾ�ڴ��з�������� 
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
    	printf("�������������£�\n");
        printf("-------- ���[%d] ---------\n",i++);
        printf("[������]:%s\n",temp->name); 
        printf("[��ַ]:0x%p\n",temp);
        printf("[����]:%d\n",temp->len);
        printf("[״̬]:%s\n",(temp->flag? "����":"��ʹ��"));
        temp=temp->next;
    }
    return;
} 

void init()//��ʼ���ڴ�block�е��ڴ�������
{
    head=(LNode *)my_memory;
    strcpy(head->name,"OSEX");
    head->next=NULL;
    head->flag=1;
    head->len=MEMSIZE-sizeof(LNode);
    show();
}

void distribute(char newname[],int newlen)//�ڴ���亯����ʹ��˳����ҵķ�ʽ�����ڴ�
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
		printf("����ʧ�ܣ�\n");
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
	printf("�ڴ����ɹ���\n");
	show();
	return;
}

void reclaim(char newname[])//�ڴ���պ��� 
{
	LNode *a,*b;
	a=head;
	temp=head; 
	while(strcmp(temp->name,newname)!=0 && temp) 
		temp=temp->next;
	if(!temp || temp->flag==1)
	{
		printf("�Ҳ��������񣬻���ʧ�ܣ�\n");
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
	printf("�ڴ���ճɹ���\n");
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
		printf(" 1 --- ��������\n 2 --- ��������\n 3 --- �˳�\n");
		printf("������ѡ���");
		scanf("%d",&selection);
		switch(selection)
		{
			case 1:
				printf("��������������");
				scanf("%s",newname);
				printf("����������������ڴ��ֽ�����");
				scanf("%d",&newlen);
				distribute(newname,newlen);
				break;
			case 2:
				printf("������Ҫ���յ���������");
				scanf("%s",newname);
				reclaim(newname);
				break;
			case 3:
				printf("���������\n");
				exit(0);
			default:
				printf("�޴�ѡ�\n");
		}
	}
	return 0;
}

