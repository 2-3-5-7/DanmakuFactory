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
7 �ļ�δ�ܰ���ȷ��ʽ���� 
*/
int readXml(const char *ipFile, DANMAKU **head, const char *mode, const float timeShift)
{
	FILE *ipF;
	DANMAKU *now = NULL, *last = NULL;
	int cnt;
	
	/*���ļ�*/
	if((ipF = fopen(ipFile, "r")) == NULL)
	{
		#if PRINT_ERR == TRUE
		printf("\n[X] �ļ���ʧ��"); 
		#endif
		return 1;
	}
	
	/*�ж϶��뷽ʽ*/
	if(*head == NULL || *mode == 'n')
	{/*�½�ģʽ*/
		freeList(*head);
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
				errorExit(ipF, *head, last);
				#if PRINT_ERR == TRUE
				printf("\n[X] ���ļ���������"); 
				#endif
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
				errorExit(ipF, *head, last);
				#if PRINT_ERR == TRUE
				printf("\n[X] ���ļ���������"); 
				#endif
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
				errorExit(ipF, *head, last);
				#if PRINT_ERR == TRUE
				printf("\n[X] ���ļ���������"); 
				#endif
				return 4;
			}
			cnt++;
		}
		t_text[cnt] = '\0';
		
		/*����һ���ڵ�Ŀռ�*/
		if((now = (DANMAKU *)malloc(sizeof(DANMAKU))) == NULL)
		{
			errorExit(ipF, *head, last);
			#if PRINT_ERR == TRUE
			printf("\n[X] �����ڴ�ռ�ʧ��"); 
			#endif
			return 5;
		}
		/*�����ı����ֵĿռ�*/ 
		if((now -> text = (char *)malloc(strlen(t_text) + 1)) == NULL)
		{
			errorExit(ipF, *head, now);
			#if PRINT_ERR == TRUE
			printf("\n[X] �����ڴ�ռ�ʧ��"); 
			#endif
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
		if(t_time + timeShift < TIME_EPS)
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
	if (now != NULL)
	{
		now -> next = NULL;
	}
	else
	{
		#if PRINT_ERR == TRUE
		printf("\n[X] �ļ����ܰ���ȷ��ʽ����"); 
		#endif
		return 7;
	}
	fclose(ipF);
	return 0;
}

/*
���������˳�ǰ�Ĵ���
������ 
�ļ�ָ��/����ͷָ��/���һ��δ��β�ڵ�ָ��/
*/
void static errorExit(FILE *ipF, DANMAKU *head, DANMAKU *ptr)
{
	fclose(ipF);
	if(head != NULL)
	{
		ptr -> next = NULL;
		freeList(head);
	}
	return;
}
