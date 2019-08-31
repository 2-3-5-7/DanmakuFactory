/*
Copyright 2019 TIKM(github:HITIKM)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
������������Ͱ����
������
Ҫ���������ͷ
����ֵ��
0 �����˳�
1 ��Ļ��Ϊ�� 
2 Ͱ�ռ�����ʧ�� 
*/
int sortList(DANMAKU **listHead)
{
	if(*listHead == NULL)
	{
		#if PRINT_ERR == TRUE
		printf("\n[X] ��Ļ��Ϊ��");
		#endif
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
		#if PRINT_ERR == TRUE
		printf("\n[X] �����ڴ�ռ�ʧ��");
		#endif
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
void freeList(DANMAKU *listHead)
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

/*
ת��������Ļ�ص��ı�����
������
����ͷ/Ŀ�����/
Ŀǰ��֧��ANSI��UTF-8��ת 
*/
void transListCoding(DANMAKU *listHead, UINT targetCoding)
{
	if (listHead == NULL)
	{
		return;
	}
	DANMAKU *nowPtr = listHead;int cnt = 0;
	while (nowPtr != NULL)
	{
		if (targetCoding == UTF_8 && isUtf8(nowPtr -> text) == FALSE)
		{
			transcoding(ANSI, UTF_8, nowPtr -> text, NULL, MAX_TEXT_LENGTH);
		}
		else if (targetCoding == ANSI && isUtf8(nowPtr -> text) == TRUE)
		{
			transcoding(UTF_8, ANSI, nowPtr -> text, NULL, MAX_TEXT_LENGTH);
		}
		nowPtr = nowPtr -> next;
	}
}
