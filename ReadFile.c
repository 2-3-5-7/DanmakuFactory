#include "DanmakuFactory.h"

/*��Ļ�����ض���淶*/
/*+----------+---------+*/
/*| �������� |  ���   |*/
/*+----------+---------+*/
/*| ������� |    1    |*/
/*+----------+---------+*/
/*| ���ҹ��� |    2    |*/
/*+----------+---------+*/
/*| �Ϸ��̶� |    3    |*/
/*+----------+---------+*/
/*| �·��̶� |    4    |*/
/*+----------+---------+*/
/*| ���ⵯĻ |    5    |*/
/*+----------+---------+*/


/*
��ȡxml�ļ����뵯Ļ�� 
������
�ļ���/����ͷ/��ȡģʽ��"n"����½� / "a"β��׷�ӣ�/ʱ��ƫ���� 
����ֵ��
0 �����˳�
1 ���ļ�ʧ�� 
2 3 4 ��ȡ�ļ���������
5 �ڵ�ռ�����ʧ��
6 �ı����ֿռ�����ʧ�� 
*/
int ReadXml(const char *ipFile, DANMAKU **head, const char *mode, const float timeShift)
{
	FILE *ipF;
	DANMAKU *now = NULL, *last = NULL;
	int cnt;
	
	/*���ļ�*/
	if((ipF = fopen(ipFile, "r")) == NULL)
	{
		return 1;
	}
	
	/*�ж϶��뷽ʽ*/
	if(*head == NULL || *mode == 'n')
	{/*�½�ģʽ*/
		FreeList(*head);
		*head = NULL;
	}
	else if(*mode == 'a')
	{/*׷��ģʽ*/
		now = last = *head;
		if((*head) -> next != NULL)
		{
			while((last -> next) -> next != NULL)
			{
				last = last -> next;
			}
			now = last -> next;
			last -> next = now;
		}
	}
	
	/*��ȡxml�ļ�*/
	float t_time;
	short t_type;
	short t_fontSize;
	int t_color;
	char ch, t_text[MAX_TEXT_LENGTH];
	float max, min;
	BOOL sort = FALSE;
	int num = 0;
	int n1Total = 0, n6Total = 0, n4Total = 0, n5Total = 0, n7Total = 0, allTotal = 0; 
	
	while(!feof(ipF))
	{
		/*Ѱ��ÿ����Ļ�Ŀ�ʼ*/
		while(fgetc(ipF) != '<' || fgetc(ipF) != 'd')
		{
			if(feof(ipF))
			{
				goto ENDREAD;
			}
			if(ferror(ipF))
			{
				ErrorExit(ipF, *head, last);
				return 2;
			}
		}
		
		fscanf(ipF, " p=\"%f,%hd,%hd,%d,", &t_time, &t_type, &t_fontSize, &t_color);
		
		/*Ѱ���ı����ֵĿ�ͷ*/
		while(fgetc(ipF) != '>')
		{
			if(feof(ipF))
			{
				goto ENDREAD;
			}
			if(ferror(ipF))
			{
				ErrorExit(ipF, *head, last);
				return 3;
			}
		}
		
		/*��ȡ�ı�����*/ 
		cnt = 0;
		while((ch = fgetc(ipF)) != '<' && cnt < MAX_TEXT_LENGTH - 1)
		{
			t_text[cnt] = ch;
			if(feof(ipF))
			{
				break;
			}
			if(ferror(ipF))
			{
				ErrorExit(ipF, *head, last);
				return 4;
			}
			cnt++;
		}
		t_text[cnt] = '\0';
		
		/*����һ���ڵ�Ŀռ�*/
		if((now = (DANMAKU *)malloc(sizeof(DANMAKU))) == NULL)
		{
			ErrorExit(ipF, *head, last);
			return 5;
		}
		/*�����ı����ֵĿռ�*/ 
		if((now -> text = (char *)malloc(strlen(t_text) + 1)) == NULL)
		{
			ErrorExit(ipF, *head, now);
			return 6;
		}
		
		if(*head == NULL)
		{
			*head = last = now;
		}
		
		/*+----------+-----------+-----------+*/
		/*| �������� | ԭ���ͱ��| �����ͱ��|*/
		/*+----------+-----------+-----------+*/
		/*| ������� |     1     |     1     |*/
		/*+----------+-----------+-----------+*/
		/*| ���ҹ��� |     6     |     2     |*/
		/*+----------+-----------+-----------+*/
		/*| �Ϸ��̶� |     5     |     3     |*/
		/*+----------+-----------+-----------+*/
		/*| �·��̶� |     4     |     4     |*/
		/*+----------+-----------+-----------+*/
		/*| ���ⵯĻ |     7     |     5     |*/
		/*+----------+-----------+-----------+*/
		switch(t_type)
		{/*��xml��������ͱ��ת��Ϊ����ͳһ��������ͱ�ţ����ϱ�*/ 
			case 1:
			{
				now -> type = 1;
				break;
			}
			case 6:
			{
				now -> type = 2;
				break;
			}
			case 5:
			{
				now -> type = 3;
				break;
			}
			case 4:
			{
				now -> type = 4;
				break;
			}
			case 7:
			{
				now -> type = 5;
				break;
			}
			default:
			{
				now -> type = t_type;
				break;
			}
		}
		
		/*���ʱ���ƫ�����Ǹ�������0*/
		if(t_time + timeShift < -TIME_EPS)
		{
			now -> time = 0.00;
		}
		else
		{
			now -> time = t_time + timeShift;
		}
		now -> fontSize = t_fontSize;
		now -> color = t_color;
		strcpy(now -> text, t_text);
		
		last -> next = now;
		last = now;
	}/*���� while*/
	ENDREAD:
	now -> next = NULL;
	fclose(ipF);
	return 0;
}

/*
���������˳�ǰ�Ĵ���
������ 
�ļ�ָ��/����ͷָ��/���һ��δ��β�ڵ�ָ��/
*/
void ErrorExit(FILE *ipF, DANMAKU *head, DANMAKU *ptr)
{
	close(ipF);
	if(head != NULL)
	{
		ptr -> next = NULL;
		FreeList(head);
	}
	return;
}
