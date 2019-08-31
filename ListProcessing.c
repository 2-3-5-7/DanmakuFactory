#include "DanmakuFactory.h"

/*
Ͱ����������������
������
Ҫ���������ͷ
����ֵ��
0 �����˳�
1 ����ָ��Ϊ��
2 Ͱ�ռ�����ʧ�� 
*/
int SortList(DANMAKU **listHead)
{
	if(*listHead == NULL)
	{
		return 1;
	}
	
	DANMAKU **bucket = NULL, *now = NULL, *last = NULL, *tail = NULL, *ptr = NULL;
	int cnt, index, danmakuNum = 0, bucketNum = 0;
	float max, min;
	
	/*ͳ�Ƶ�Ļ�������ҳ������Сֵ*/
	now = *listHead;
	while(now != NULL)
	{
		if(now == *listHead)
		{
			max = min = (*listHead) -> time;
		}
		else
		{
			if(now -> time > max)
			{
				max = now -> time;
			}
			else if(now -> time < min)
			{
				min = now -> time;
			}
		}
		danmakuNum++;
		now = now -> next;
	}
	
	/*����Ͱ�ռ䲢��0*/
	now = *listHead;
	bucketNum = danmakuNum / 128 + 1;
	if((bucket = (DANMAKU **)malloc(sizeof(DANMAKU *) * bucketNum)) == NULL)
	{
		return 2;
	}
	memset(bucket, 0, sizeof(DANMAKU *) * bucketNum);
	
	/*��Ͱ*/ 
	while(*listHead != NULL)
	{
		index = (now -> time - min) / (max - min + 1) * bucketNum;
		*listHead = (*listHead) -> next;
		if(bucket[index] == NULL)
		{/*�����ͰΪ�����½ڵ�ָ������*/ 
			bucket[index] = now;
			now -> next = NULL;
		}
		else
		{
			ptr = last = bucket[index];
			if(now -> time < ptr -> time)
			{/*�ж��Ƿ�Ϊ��Ͱ��Сֵ*/ 
				now -> next = ptr;
				bucket[index] = now;
			}
			else
			{
				ptr = ptr -> next;
				while(ptr != NULL && now -> time > ptr -> time - TIME_EPS)
				{/*��ptr != NULL && now->time >= ptr->time*/
					last = ptr;
					ptr = ptr -> next;
				}
				now -> next = ptr;
				last -> next = now;
			}
		}
		now = *listHead;
	}/*���� while*/
	
	/*��Ͱ*/
	now = *listHead = NULL;
	for(cnt = 0; cnt < bucketNum; cnt++)
	{
		ptr = bucket[cnt];
		
		if(ptr != NULL)
		{
			if(*listHead == NULL)
			{
				*listHead = now = ptr;
			}
			else
			{
				now -> next = ptr;
			}
			while(ptr -> next != NULL)
			{
				ptr = ptr -> next;
			}
			now = ptr;
		}
	}
	free(bucket);
	return 0; 
}


/*
�ͷ���������
������
Ҫfree��������ͷ
*/
void FreeList(DANMAKU *listHead)
{
	DANMAKU *ptr = listHead;
	while(ptr != NULL)
	{
		listHead = ptr -> next;
		free(ptr -> text);/*�ͷ��ı����ֵĿռ�*/ 
		free(ptr);
		ptr = listHead;
	}
}
