#include "DanmakuFactory.h"

/*
�����ӿڣ� 
�����ļ�/����ļ�/�ֱ��ʿ�/�ֱ��ʸ�/�ֺ�/����/
��Ӱ/���/�����ٶȣ�������Ļ��/ͣ��ʱ�䣨������Ļ��/�ܶ�/��͸����/�²�����/
���Σ�����/����/����/�ײ�/����/��ɫ/�ظ���/����ģʽ�����ݱ��/�ֲ�ͼ��NULL��ʾ�ݲ�����debugģʽ/
*/
int WriteAss(const char *opFileName, DANMAKU *head, const int resX, int resY, const int fontSize,
			 char *fontName,const int shadow, const int outline, const float speed, const float holdTime, 
			 const int density, const int opacity, const int blank, char *shieldMode, char *debugMode)
{
	if(head == NULL)
	{
		return 1;
	}
	
	FILE *opF;
	DANMAKU *now = NULL, *last = NULL;
	int cnt;
	
	/*���ļ�*/
	if((opF = fopen(opFileName, "w")) == NULL)
	{
		return 2;
	}
	
	char tempText[MAX_TEXT_LENGTH];
	DANMAKU *signPtr = head, *scanPtr = head;
	
	int screenNum;
	int listCnt, densityCnt, PositionY;
	int textLen, textHei;
	
	float *R2LToRightTime, *R2LToLeftTime;	/*��������о����ض���ʱ��*/
	float *L2RToRightTime, *L2RToLeftTime;	/*���ҹ����о����ض���ʱ��*/
	float *fixEndTime;	/*������ײ���Ļ��ʧʱ��*/
	
	if((R2LToRightTime = (float *)malloc(resY * sizeof(float))) == NULL)
	{
		return 3;
	}
	if((R2LToLeftTime = (float *)malloc(resY * sizeof(float))) == NULL)
	{
		free(R2LToRightTime);
		return 4;
	}
	if((L2RToRightTime = (float *)malloc(resY * sizeof(float))) == NULL)
	{
		free(R2LToRightTime);
		free(R2LToLeftTime);
		return 5;
	}
	if((L2RToLeftTime = (float *)malloc(resY * sizeof(float))) == NULL)
	{
		free(R2LToRightTime);
		free(R2LToLeftTime);
		free(L2RToRightTime);
		return 6;
	}
	if((fixEndTime = (float *)malloc(resY * sizeof(float))) == NULL)
	{
		free(R2LToRightTime);
		free(R2LToLeftTime);
		free(L2RToRightTime);
		free(L2RToLeftTime);
		return 7;
	}
	
	/*дassͷ*/
	char hexOpacity[3];
	ToHexOpacity(255 - opacity, hexOpacity);
	fprintf(opF, "[Script Info]\n"
				 "ScriptType: v4.00+\n"
				 "Collisions: Normal\n"
				 "PlayResX: %d\n"
				 "PlayResY: %d\n"
				 "Timer: 100.0000\n\n" 
				 "[V4+ Styles]\n"
				 "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, "
				 "OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, "
				 "ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, "
				 "MarginL, MarginR, MarginV, Encoding\n"
				 "Style: Default,%s,%d,&H%sFFFFFF,&H1FFFFFF,&H1000000,"
				 "&H1E6A5149,0,0,0,0,100.00,100.00,0.00,0.00,1,%d,%d,8,0,0,0,1",
				resX, resY, fontName, fontSize, hexOpacity, outline, shadow);
	
	fprintf(opF, "\n\n[Events]\n"
				 "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text");
	
	if(blank != 0)
	{
		resY -= blank;
	}
	
	now = head;
	listCnt = 0;
	while(now != NULL)
	{/*������дass*/
		listCnt++;
		 
		/*���������μ���ɫ����*/
		if( (shieldMode[0] != '0' && now -> type == 1) || (shieldMode[1] != '0' && now -> type == 2) ||
		    (shieldMode[2] != '0' && now -> type == 3) || (shieldMode[3] != '0' && now -> type == 4) ||
		    (shieldMode[4] != '0' && now -> type == 5) || (shieldMode[5] != '0' && now -> color != 0xFFFFFF) )
		{/*������������*/
			now -> type *= -1;
			goto NEXTNODE;
		}
		
		/*�ܶ��������� �� ��ͬ���ݵ�Ļ����*/
		if(density > 0 || shieldMode[6] != '0')
		{
			while(GetEndTime(signPtr, speed, holdTime) < now -> time + TIME_EPS)
			{/*�ƶ�ָ�뵽ͬ����һ����Ļ*/
				signPtr = signPtr -> next;
			}
			
			scanPtr = signPtr;
			screenNum = 0;
			while(scanPtr != now)
			{ 
				if(scanPtr -> type > 0 && GetEndTime(scanPtr, speed, holdTime) > now -> time)
				{
					if(IS_NORMAL(scanPtr) && now -> type > 0 &&
					   shieldMode[6] != '0' && !strcmp(scanPtr -> text, now -> text))
					{/*����ظ��������ο����������ظ�*/
						now -> type *= -1;
						goto NEXTNODE;
					}
					
					if(IS_NORMAL(scanPtr) || IS_SPECIAL(scanPtr))
					{/*�����һ���Ϸ��ĵ�Ļ*/
						screenNum++;
					}
				}
				scanPtr = scanPtr -> next;
			}
			
			if(now -> type > 0 && density > 0 && density <= screenNum)
			{/*�ж��Ƿ�ﵽ��Ļ�ܶ�Ҫ�����޲�����*/
				now -> type *= -1;
				//goto NEXTNODE;
			}
		}
		
		textLen = GetStrLen(now -> text, fontSize, now -> fontSize, fontName);
		textHei = GetStrHei(now -> text, fontSize, now -> fontSize, fontName);
		if(now -> type == 1)/*����Ļ*/ 
		{
			for(PositionY = 1; PositionY < resY - textHei; PositionY++)
			{
				for(cnt = 0; cnt < fontSize; cnt++)
				{
					if(now->time < R2LToRightTime[PositionY + cnt] || 
					   now->time + speed*(float)resX/(resX+textLen) < R2LToLeftTime[PositionY + cnt])
					{/*��������Ļ���ָ������һ����Ļδ�뿪��Ļ�ұ� �� 
						��������Ļ�������ʱ�������һ����Ļû����ȫ�˳���Ļ*/
						PositionY = PositionY + cnt + 1;
						break;
					}
				}
				if(cnt >= textHei)
				{
					break;
				}
			}
			if(PositionY >= resY - textHei)
			{
				if(density == -1)
				{
					now -> type == -1;
					goto NEXTNODE;
				}
				PositionY = FindMin(R2LToRightTime, resY, resY - textHei, 0);
			}
			for(cnt = 0; cnt < textHei; cnt++)
			{
				R2LToRightTime[PositionY + cnt] = now -> time + speed * (float)textLen / (resX + textLen); 
				R2LToLeftTime[PositionY + cnt] = now -> time + speed;
			}
			
			fprintf(opF, "\nDialogue: 0,");
			PrintTime(opF, now->time, ",");
			PrintTime(opF, now->time + speed, ",");
			fprintf(opF, "Default,,0000,0000,0000,,{\\move(%d,%d,%d,%d)\\q2",
					resX + textLen/2, PositionY, -1 * textLen / 2, PositionY);
			
			if(now -> fontSize != 25)
			{
				fprintf(opF, "\\fs%d", textHei);
			}
			
			if(now -> color != 0xFFFFFF)
			{
				char hexColor[7];
				fprintf(opF, "\\c&H%s", ToHexColor(now->color, hexColor));
			}
			
			fprintf(opF, "}%s", now->text);
		}
		else if(now -> type == 2)/*���ҵ�Ļ*/ 
		{
			for(PositionY = 1; PositionY < resY - textHei; PositionY++)
			{
				for(cnt = 0; cnt < textHei; cnt++)
				{
					if(now->time < L2RToRightTime[PositionY + cnt] || 
					   now->time + speed*(float)resX/(resX+textLen) < L2RToLeftTime[PositionY + cnt])
					{/*��������Ļ���ָ������һ����Ļδ�뿪��Ļ��� �� 
						��������Ļ�����Ҷ�ʱ�������һ����Ļû����ȫ�˳���Ļ*/
						PositionY = PositionY + cnt + 1;
						break;
					}
				}
				if(cnt >= textHei)
				{
					break;
				}
			}
			if(PositionY >= resY - textHei)
			{
				if(density == -1)
				{
					now -> type == -2;
					goto NEXTNODE;
				}
				PositionY = FindMin(L2RToRightTime, resY, resY - textHei, 0);
			}
			for(cnt = 0; cnt < textHei; cnt++)
			{
				L2RToRightTime[PositionY + cnt] = now -> time + speed * (float)textLen / (resX + textLen); 
				L2RToLeftTime[PositionY + cnt] = now -> time + speed;
			}
			
			fprintf(opF, "\nDialogue: 0,");
			PrintTime(opF, now->time, ",");
			PrintTime(opF, now->time + speed, ",");
			fprintf(opF, "Default,,0000,0000,0000,,{\\move(%d,%d,%d,%d)\\q2",
					-1 * textLen / 2, PositionY, resX + textLen/2, PositionY);
			
			if(now -> fontSize != 25)
			{
				fprintf(opF, "\\fs%d", textHei);
			}
			
			if(now -> color != 0xFFFFFF)
			{
				char hexColor[7];
				fprintf(opF, "\\c&H%s", ToHexColor(now->color, hexColor));
			}
			
			fprintf(opF, "}%s", now->text);
		}
		else if(now -> type == 3)/*���˵�Ļ*/ 
		{
			for(PositionY = 1; PositionY < resY - textHei; PositionY++)
			{
				for(cnt = 0; cnt < textHei; cnt++)
				{
					if(now->time < fixEndTime[PositionY + cnt])
					{/*��������Ļ����ʱ������һ����Ļ��û����ʧ*/
						PositionY = PositionY + cnt + 1;
						break;
					}
				}
				if(cnt >= textHei)
				{
					break;
				}
			}
			if(PositionY >= resY - textHei)
			{
				if(density == -1)
				{
					now -> type == -3;
					goto NEXTNODE;
				}
				PositionY = FindMin(fixEndTime, resY, resY - textHei, 0);
			}
			for(cnt = 0; cnt < textHei; cnt++)
			{
				fixEndTime[PositionY + cnt] = now -> time + holdTime;
			}
			
			fprintf(opF, "\nDialogue: 0,");
			PrintTime(opF, now->time, ",");
			PrintTime(opF, now->time + holdTime, ",");
			fprintf(opF, "Default,,0000,0000,0000,,{\\pos(%d,%d)\\q2", resX / 2, PositionY);
			
			if(now -> fontSize != 25)
			{
				fprintf(opF, "\\fs%d", textHei);
			}
			
			if(now -> color != 0xFFFFFF)
			{
				char hexColor[7];
				fprintf(opF, "\\c&H%s", ToHexColor(now->color, hexColor));
			}
			
			fprintf(opF, "}%s", now->text);
		}
		else if(now -> type == 4)/*�׶˵�Ļ*/ 
		{
			for(PositionY = resY - 1; PositionY > textHei - 1; PositionY--)
			{
				for(cnt = 0; cnt < textHei; cnt++)
				{
					if(now->time < fixEndTime[PositionY - cnt])
					{/*��������Ļ����ʱ������һ����Ļ��û����ʧ*/
						PositionY = PositionY - cnt - 1;
						break;
					}
				}
				if(cnt >= textHei)
				{
					break;
				}
			}
			if(PositionY < textHei)
			{
				if(density == -1)
				{
					now -> type == -4;
					goto NEXTNODE;
				}
				PositionY = FindMin(fixEndTime, resY, textHei, 1);
			}
			for(cnt = 0; cnt < textHei; cnt++)
			{
				fixEndTime[PositionY - cnt] = now -> time + holdTime;
			}
			
			fprintf(opF, "\nDialogue: 0,");
			PrintTime(opF, now->time, ",");
			PrintTime(opF, now->time + holdTime, ",");
			fprintf(opF, "Default,,0000,0000,0000,,{\\pos(%d,%d)\\q2",
					resX / 2, PositionY - textHei + 2);
			
			if(now -> fontSize != 25)
			{
				fprintf(opF, "\\fs%d", textHei);
			}
			
			if(now -> color != 0xFFFFFF)
			{
				char hexColor[7];
				fprintf(opF, "\\c&H%s", ToHexColor(now->color, hexColor));
			}
			
			fprintf(opF, "}%s", now->text);
		}
		else if(now -> type == 5)/*���ⵯĻ*/
		{
			char *n7Ptr = now -> text;
			float n7ExistTime;
			int n7MoveTime, n7PauseTime;
			float n7StartX, n7StartY, n7EndX, n7EndY;
			int n7FadeStart, n7FadeEnd, n7FrY, n7FrZ;
			char n7Temp[MAX_TEXT_LENGTH], n7FontName[256];
			while(*n7Ptr != '[' && *n7Ptr++ !='\0')
			{
				n7Ptr++;
			}
			n7Ptr++;
			/*[0,0.17,"1-1",7,"�ı���������",0,0,0,0.17,500,0,true,"΢���ź�",1]*/
			n7StartX = StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7StartY = StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7FadeStart = (int)((1 - StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, '-')))) * 255);		
			n7FadeEnd = (int)((1 - StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')))) * 255);	
			n7ExistTime = StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			
			/*�ռ��ı�����*/
			while(*(n7Ptr - 1) != '\"' && *n7Ptr != '\0' && !(*(n7Ptr - 6) == '&' && *(n7Ptr - 5) == 'q' && 
				  *(n7Ptr - 4) == 'u' && *(n7Ptr - 3) == 'o' && *(n7Ptr - 2) == 't' && *(n7Ptr - 1) == ';'))
			{/*��ָ���Ƶ����Ż���ת��&quot;֮��*/
				n7Ptr++;
			}
			cnt = 0;
			while(*n7Ptr != '\"' && *n7Ptr != '\0' &&  !(*n7Ptr == '&' && *(n7Ptr + 1) == 'q' && 
				   *(n7Ptr + 2) == 'u' && *(n7Ptr + 3) == 'o' && *(n7Ptr + 4) == 't' && *(n7Ptr + 5) == ';') )
			{/*�����Ż���ת��&quot;֮ǰֹͣ����*/
				if(*n7Ptr == '/' && *(n7Ptr + 1) == 'n')
				{/*��xml��Ļ�ļ���/nΪ���з���ass��\NΪǿ�ƻ��з���ֻ�����ⵯĻ���з���Ч*/ 
					tempText[cnt] = '\\';
					tempText[cnt + 1] = 'N';
					cnt++;
					n7Ptr++;
				}
				else
				{
					tempText[cnt] = *n7Ptr;
				}
				cnt++;
				n7Ptr++;
			}
			tempText[cnt] = '\0';
			while(*(n7Ptr - 1) != ',' && *n7Ptr !='\0')
			{
				n7Ptr++;
			}
			
			n7FrZ = StrToInt(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7FrY = StrToInt(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7EndX = StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7EndY = StrToFloat(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7MoveTime = StrToInt(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			n7PauseTime = StrToInt(DeQuotMarks(StrGetLeftPart(n7Temp, &n7Ptr, ',')));
			
			while(*n7Ptr != ',' && *n7Ptr != '\0')
			{
				n7Ptr++;
			}
			
			/*������Ϣ �ռ�����ͬ�ı�����*/ 
			while(*(n7Ptr - 1) != '\"' && *n7Ptr != '\0' &&  !(*(n7Ptr - 6) == '&' && *(n7Ptr - 5) == 'q' && 
				  *(n7Ptr - 4) == 'u' && *(n7Ptr - 3) == 'o' && *(n7Ptr - 2) == 't' && *(n7Ptr - 1) == ';'))
			{
				n7Ptr++;
			}
			cnt = 0;
			while(*n7Ptr != '\"' && *n7Ptr != '\0' && cnt < 512 && !(*n7Ptr == '&' && *(n7Ptr + 1) == 'q' && 
				  *(n7Ptr + 2) == 'u' && *(n7Ptr + 3) == 'o' && *(n7Ptr + 4) == 't' && *(n7Ptr + 5) == ';'))
			{
				n7FontName[cnt] = *n7Ptr;
				cnt++;
				n7Ptr++;
			}
			n7FontName[cnt] = '\0';
			
			fprintf(opF, "\nDialogue: 0,");
			PrintTime(opF, now->time, ",");
			PrintTime(opF, now->time + n7ExistTime, ",");
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7");
			if( (n7StartX - 1 < TIME_EPS && n7StartX > TIME_EPS) || (n7EndX - 1 < TIME_EPS && n7EndX > TIME_EPS)||
				(n7StartY - 1 < TIME_EPS && n7StartY > TIME_EPS) || (n7EndY - 1 < TIME_EPS && n7EndY > TIME_EPS) )
			{
				n7StartX *= resX;
				n7EndX *= resX;
				n7StartY *= resY;
				n7EndY *= resY;
			}
			if(n7StartX == n7EndX && n7StartY == n7EndY)
			{
				fprintf(opF, "\\pos(%d,%d)", (int)n7StartX, (int)n7StartY);
			}
			else
			{
				fprintf(opF, "\\move(%d,%d,%d,%d", (int)n7StartX, (int)n7StartY, (int)n7EndX, (int)n7EndY);
				if(n7PauseTime == 0)
				{
					if(n7MoveTime == 0)
					{
						fprintf(opF, ")");
					}
					else
					{
						fprintf(opF, ",0,%d)", n7MoveTime);
					}
				}
				else
				{
					fprintf(opF, ",%d,%d)", n7PauseTime, n7MoveTime + n7PauseTime);
				}
			}
			fprintf(opF, "\\q2");
			
			if(now -> fontSize != 25)
			{
				fprintf(opF, "\\fs%d", fontSize + (now -> fontSize - 25) );
			}
			
			if(*n7Temp != '\0')
			{
				fprintf(opF, "\\fn%s", n7FontName);
			}
			
			if(now -> color != 0xFFFFFF)
			{
				char hexColor[7];
				fprintf(opF, "\\c&H%s", ToHexColor(now->color, hexColor));
			}
			
			if(n7FrY != 0)
			{
				fprintf(opF, "\\fry%d", n7FrY);
			}
			if(n7FrZ != 0)
			{
				fprintf(opF, "\\frz%d", n7FrZ);
			}
			
			if(n7FadeStart != 0 || n7FadeEnd != 0)
			{/*fade(����͸����,ʵ��͸����,����͸����,���뿪ʼʱ��,�������ʱ��,������ʼʱ��,��������ʱ��)*/
				fprintf(opF, "\\fade(%d,%d,%d,0,0,%d,%d)", n7FadeStart, n7FadeStart,
						n7FadeEnd, n7PauseTime, (int)(n7ExistTime * 1000));
			}
			else
			{
				fprintf(opF, "\\alpha&H00");
			}
			
			fprintf(opF, "}%s", tempText);
		}
		else if(now -> type == 8)/*���뵯Ļ*/ 
		{
			fprintf(opF, "\nDialogue: 0,0:00:00.00,0:00:00.00,Default,"
					"NO.%d(Code danmaku):unable to read this type,0000,0000,0000,,", listCnt);
		}
		else if(now -> type > 0)/*����Ļ*/ 
		{
			fprintf(opF, "\nComment: NO.%d:unknow type", listCnt);
		}
		
		if(ferror(opF))
		{
			return 8;
		}
		
		NEXTNODE:
		now = now -> next;
	}
	/*�黹�ռ�*/
	free(R2LToRightTime);
	free(R2LToLeftTime);
	free(L2RToRightTime);
	free(L2RToLeftTime);
	free(fixEndTime);
	
	fflush(opF);
	fclose(opF);
	
	/*������дass��ʱ��˳��дdebug���� Ҳ���Ե�������WriteAssDebugPath����*/
	if(debugMode != NULL && debugMode[0] != '0' && debugMode[1] != '0' && debugMode[2] != '0')
	{
		WriteAssDebugPath(opFileName, head, debugMode, speed, holdTime, shieldMode);
	}
	return 0;
}

/*
������
����ļ���/����ͷ/ģʽ��ͳ�Ʊ�/�ֲ�ͼ/��ǩ��/�ֱ��ʿ�/�ֱ��ʸ�/�����ٶȣ�������Ļ��/ͣ��ʱ�䣨������Ļ��/
�ֺ�/����(utf-8)/���Σ�����/����/����/�ײ�/����/��ɫ/�ظ��� 
����ֵ��
0 �����˳�
1 ����Ϊ��
2 �½��ļ�ʧ�� 
*/ 
int WriteAssDebugPath(const char *opFileName, DANMAKU *head, char *mode, const int speed, const int holdTime,
					  char *shieldMode)
{
	if(mode[0] == '0' && mode[1] == '0')
	{
		return 0;
	}
	
	if(head == NULL)
	{
		return 1;
	}
	
	FILE *opF;
	DANMAKU *now = NULL, *last = NULL;
	int cnt;
	
	/*���ļ�*/
	if((opF = fopen(opFileName, "a+")) == NULL)
	{
		return 2;
	}
	
	/*���������±��ʾ��Ļ���ͣ������±�0��ʾ��������ֵ��¼�˵�Ļ�����������������������ѭ��*/ 
	int screen[NUM_OF_TYPE + 1] = {0};/*ͬ����Ļ*/ 
	int shield[NUM_OF_TYPE + 1] = {0};/*�����εĵ�Ļ*/ 
	int count[NUM_OF_TYPE + 1] = {0};/*δ���ε�Ļ������*/ 
	int total[NUM_OF_TYPE + 1] = {0};/*�ܵ�Ļ����*/
	
	int pointShiftY = 0;/*Y����Ա�׼λ������ƫ�Ƶĸ߶�*/
	float startTime, endTime;/*debug������¼�Ŀ�ʼ�����ʱ��*/
	float lastDanmakuStartTime = 0.00, lastDanmakuEndTime = 0.00;/*���һ����Ļ�ĳ�������ʧ��ʱ��*/
	
	DANMAKU *signPtr = NULL, *scanPtr = NULL;
	
	fprintf(opF, "\n\nComment:DebugPath");
	fprintf(opF, "\nComment:MODE:%s", mode);
	
	if(mode[0] == '0')
	{/*�������Ҫ���ݿ򣬷ֲ�ͼ��Ҫ����*/ 
		pointShiftY = -255; 
	}
	
	/*����Ļ�ֲ�ͼ*/
	if(mode[1] != 0)
	{
		int mPointX;/*��ͼʱx����ı���*/
		
		int chartMaxHeight = 0;/*ͼ����ߵ�ĸ߶�*/ 
		int allCnt[212] = {0}, shieldCnt[212] = {0};
		 
		BOOL drawShieldChart = FALSE;
		
		/*������൯Ļ����*/
		now = head;
		while(now != NULL)
		{
			if(abs(now -> type) <= 5)
			{
				total[abs(now -> type)]++;
			}
			
			if(now -> next == NULL)
			{
				lastDanmakuStartTime = now -> time;
				lastDanmakuEndTime = GetEndTime(now, speed, holdTime);
			}
			now = now -> next;
		}
		/*ʹ��0λ�ô洢����*/
		total[0] = total[1] + total[2] + total[3] + total[4] + total[5];
		
		endTime = lastDanmakuStartTime + 30;
		/*����ƫ����ѡ����ȷ�ķֲ�ͼ��*/
		if(pointShiftY == 0)
		{
			fprintf(opF, "\nDialogue: 5,0:00:00.00,");
			PrintTime(opF, endTime, ",");
			/*�׿�*/ 
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\1c&HEAF3CE\\1a&H35\\shad0\\bord0\\p1}m 20 290"
						 " b 20 284 29 275 35 275 l 430 275 b 436 275 445 284 445 290 l 445 370"
						 " b 445 376 436 385 430 385 l 35 385 b 29 385 20 376 20 370 l 20 290{\\p0}");
			
			fprintf(opF, "\nDialogue: 6,0:00:00.00,");
			PrintTime(opF, lastDanmakuEndTime, ",");
			/*�ƶ��Ľ�����*/
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\move(20, 275, 444, 275)\\clip(m 20 290 b 20 284 29"
						 " 275 35 275 l 430 275 b 436 275 445 284 445 290 l 445 370 b 445 376 436 385 430 385"
						 " l 35 385 b 29 385 20 376 20 370 l 20 290)\\1c&HCECEF3\\1a&H35\\shad0\\bord0\\p1}"
						 "m 0 0 l -425 0 l -425 110 l 0 110 l 0 0{\\p0}");
			fprintf(opF, "\nDialogue: 6,");
			PrintTime(opF, lastDanmakuEndTime, ",");
			PrintTime(opF, endTime, ",");
			/*��������֮�������*/
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\pos(445, 275)\\clip(m 20 290 b 20 284 29 275 35 275"
						 " l 430 275 b 436 275 445 284 445 290 l 445 370 b 445 376 436 385 430 385 l 35 385"
						 " b 29 385 20 376 20 370 l 20 290)\\1c&HCECEF3\\1a&H35\\shad0\\bord0\\p1}"
						 "m 0 0 l -425 0 l -425 110 l 0 110 l 0 0{\\p0}");
		}
		else if(pointShiftY == -255)
		{
			fprintf(opF, "\nDialogue: 5,0:00:00.00,");
			PrintTime(opF, endTime, ",");
			/*�׿�*/ 
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\1c&HEAF3CE\\1a&H35\\shad0\\bord0\\p1}m 20 35"
						 " b 20 29 29 20 35 20 l 430 20 b 436 20 445 29 445 35 l 445 115"
						 " b 445 121 436 130 430 130 l 35 130 b 29 130 20 121 20 115 l 20 35{\\p0}");
			
			fprintf(opF, "\nDialogue: 6,0:00:00.00,");
			PrintTime(opF, lastDanmakuEndTime, ",");
			/*�ƶ��Ľ�����*/
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\move(20, 20, 444, 20)\\clip(m 20 35 b 20 29 29 20"
						 " 35 20 l 430 20 b 436 20 445 29 445 35 l 445 115 b 445 121 436 130 430 130 l 35 130"
						 " b 29 130 20 121 20 115 l 20 35)\\1c&HCECEF3\\1a&H35\\shad0\\bord0\\p1}"
						 "m 0 0 l -425 0 l -425 110 l 0 110 l 0 0{\\p0}");
			fprintf(opF, "\nDialogue:6,");
			PrintTime(opF, lastDanmakuEndTime, ",");
			PrintTime(opF, endTime, ",");
			/*��������֮�������*/
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\pos(445, 20)\\clip(m 20 35 b 20 29 29 20"
						 " 35 20 l 430 20 b 436 20 445 29 445 35 l 445 115 b 445 121 436 130 430 130 l 35 130"
						 " b 29 130 20 121 20 115 l 20 35)\\1c&HCECEF3\\1a&H35\\shad0\\bord0\\p1}"
						 "m 0 0 l -425 0 l -425 110 l 0 110 l 0 0{\\p0}");
		}
		
		
		now = head;
		while(now != NULL)
		{/*ͳ�Ƹ�ʱ��ε�Ļ����*/
			for(cnt = (int)(now->time / lastDanmakuEndTime * 211); 
				cnt < (int)(GetEndTime(now, speed, holdTime) / lastDanmakuEndTime * 211); cnt++)
			{/*�ӿ�ʼʱ��д������ʱ��*/
				allCnt[cnt]++;
				if(now -> type < 0)
				{
					shieldCnt[cnt]++;
					drawShieldChart = TRUE;
				}
			}
			now = now -> next;
		}
		
		for(cnt = 0; cnt < 212; cnt++)
		{/*Ѱ�����߶�*/ 
			if(allCnt[cnt] > chartMaxHeight)
			{
				chartMaxHeight = allCnt[cnt];
			}
		}
		
		/*��ȫ����Ļ�ֲ�ͼ*/
		fprintf(opF, "\nDialogue:7,0:00:00.00,");
		PrintTime(opF, endTime, ",");
		if(pointShiftY == 0)
		{/*�ֲ�ͼ��ʼ��*/ 
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\clip(m 20 290 b 20 284 29 275 35 275 l 430 275"
						 " b 436 275 445 284 445 290 l 445 370 b 445 376 436 385 430 385 l 35 385"
						 " b 29 385 20 376 20 370 l 20 290)\\1c&HFFFFFF\\1a&H35\\shad0\\bord0\\p1}m 20 385");
		}
		else if(pointShiftY == -255)
		{
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\clip(m 20 35 b 20 29 29 20"
						 " 35 20 l 430 20 b 436 20 445 29 445 35 l 445 115 b 445 121 436 130 430 130 l 35 130"
						 " b 29 130 20 121 20 115 l 20 35)\\1c&HFFFFFF\\1a&H35\\shad0\\bord0\\p1}m 20 130");
		}
		
		mPointX = 21;
		for(cnt = 0; cnt < 212; cnt++)
		{/*����ͳ�����ݻ�ͼ����߸߶���110px*/ 
			if(cnt == 1)
			{/*��һ����*/ 
				fprintf(opF, " l %d %d",
						mPointX, 385 - (int)((float)allCnt[cnt] / chartMaxHeight * 110) + pointShiftY);
				mPointX += 2;
			}
			
			/*��������������һ�����*/ 
			fprintf(opF, " l %d %d",
						mPointX, 385 - (int)((float)allCnt[cnt] / chartMaxHeight * 110) + pointShiftY);
			
			
			if(cnt == 211)
			{/*���һ�������ӵ�ͼ�����½�*/ 
				fprintf(opF, " l %d %d", mPointX, 385 + pointShiftY);
			}
			else
			{/*�����ߣ�������������һ�����ݵ�߶�*/ 
				fprintf(opF, " l %d %d",
						mPointX, 385 - (int)((float)allCnt[cnt + 1] / chartMaxHeight * 110) + pointShiftY);
				mPointX += 2;
			}
		}
		
		/*���ͼ��*/
		fprintf(opF, " l 20 385{\\p0}");
		
		/*�����ε�Ļ�ֲ�ͼ*/ 
		if(drawShieldChart == TRUE)
		{
			mPointX = 21;
			fprintf(opF, "\nDialogue: 8,0:00:00.00,");
			PrintTime(opF, endTime, ",");
			
			if(pointShiftY == 0)
			{
				fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\clip(m 20 290 b 20 284 29 275 35 275 l 430 275"
							 " b 436 275 445 284 445 290 l 445 370 b 445 376 436 385 430 385 l 35 385"
							 " b 29 385 20 376 20 370 l 20 290)\\1c&HD3D3D3\\1a&H35\\shad0\\bord0\\p1}m 20 385");
			}
			else if(pointShiftY == -255)
			{
				fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\clip(m 20 35 b 20 29 29 20"
							 " 35 20 l 430 20 b 436 20 445 29 445 35 l 445 115 b 445 121 436 130 430 130 l 35 130"
							 " b 29 130 20 121 20 115 l 20 35)\\1c&HD3D3D3\\1a&H35\\shad0\\bord0\\p1}m 20 130");
			}
			for(cnt = 0; cnt < 212; cnt++)
			{/*����ͳ�����ݻ�ͼ����߸߶���110px*/ 
				if(cnt == 1)
				{/*��һ����*/ 
					fprintf(opF, " l %d %d",
							mPointX, 385 - (int)((float)shieldCnt[cnt] / chartMaxHeight * 110) + pointShiftY);
					mPointX += 2;
				}
				
				/*��������������һ�����*/
				fprintf(opF, " l %d %d",
							mPointX, 385 - (int)((float)shieldCnt[cnt] / chartMaxHeight * 110) + pointShiftY);
				
				if(cnt == 211)
				{/*���һ�������ӵ�ͼ�����½�*/
					fprintf(opF, " l %d %d", mPointX, 385 + pointShiftY);
				}
				else
				{/*�����ߣ�������������һ�����ݵ�߶�*/ 
					fprintf(opF, " l %d %d",
							mPointX, 385 - (int)((float)shieldCnt[cnt + 1] / chartMaxHeight * 110) + pointShiftY);
					mPointX += 2;
				}
			}
			/*���ͼ��*/
			fprintf(opF, " l 20 385{\\p0}");
		}
	}
	
	/*��ʾ��Ļͳ�Ʊ�*/
	if(mode[0] != '0')
	{
		now = head;
		if(!total[0])
		{/*ͳ�Ƴ�ÿ�ֵ�Ļ������ ���֮ǰû�м���������һ��*/
			while(now != NULL)
			{
				if(abs(now -> type) <= NUM_OF_TYPE)
				{
					total[abs(now -> type)]++;
				}
				
				if(now -> next == NULL)
				{
					lastDanmakuStartTime = now -> time;
				}
				now = now -> next;
			}
			/*ʹ��0λ�ô洢����*/
			total[0] = total[1] + total[2] + total[3] + total[4] + total[5];
		}
		
		/*�������״������ĳ�פ���ʼ����*/
		endTime = lastDanmakuStartTime + 30;
		fprintf(opF, "\n\nDialogue:3,0:00:00.00,");
		PrintTime(opF, endTime, ",");
		fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\1c&HCECEF3\\1a&H35\\shad0\\bord0\\p1}"
					 "m 20 35 b 20 26 26 20 35 20 l 430 20 b 439 20 445 29 445 35 l 445 50 l 20 50 l 20 35 "
					 "m 20 80 l 445 80 l 445 110 l 20 110 l 20 110 l 20 80 m 20 140 l 445 140 l 445 170 l 20"
					 " 170 l 20 170 l 20 140 m 20 200 l 445 200 l 445 230 l 20 230 l 20 230 l 20 200{\\p0}");
		fprintf(opF, "\nDialogue:4,0:00:00.00,");
		PrintTime(opF, endTime, ",");
		fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\1c&HEAF3CE\\1a&H35\\shad0\\bord0\\p1}"
					 "m 20 50 l 445 50 l 445 80 l 20 80 l 20 80 l 20 50 m 20 110 l 445 110 l 445 140 l 20 140"
					 " l 20 140 l 20 110 m 20 170 l 445 170 l 445 200 l 20 200 l 20 200 l 20 170 m 20 230"
					 " l 445 230 l 445 230 l 445 245 b 445 251 436 260 430 260 l 35 260 b 29 260 20 251 20 245"
					 " l 20 230{\\p0}");
		
		/*��ʾ������� ע*����ģʽ������ͳһ�ڵ�ʮ����ʾ*/
		PrintDebugDataStr(opF, 0.00, endTime,  62, 35, NULL, "type");
		PrintDebugDataStr(opF, 0.00, endTime, 147, 35, NULL, "screen");
		PrintDebugDataStr(opF, 0.00, endTime, 232, 35, NULL, "shieldCnt");
		PrintDebugDataStr(opF, 0.00, endTime, 317, 35, NULL, "count");
		PrintDebugDataStr(opF, 0.00, endTime, 402, 35, NULL, "total");
		fprintf(opF, "\nDialogue:10,0:00:00.00,");
		PrintTime(opF, endTime, ",");
		fprintf(opF, "Default,,0000,0000,0000,,{\\an4\\pos(35,245)\\b1\\fs25\\1c&HFFFFFF"
					 "\\1a&H35\\shad1\\bord0}DanmakuFactory v%s", VERSION);
		
		/*��ʾ��Ļ���ͱ�ͷ����������ξ�ʹ�û���*/
		if(shieldMode[0] == '0' || shieldMode[1] == '0' || shieldMode[2] == '0' ||
		   shieldMode[3] == '0' || shieldMode[4] == '0')
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 65, NULL, "ALL");
		}
		else
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 65, "\\s1", "ALL");
		}
		if(shieldMode[0] == '0')
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 95, NULL, "R to L");
		}
		else
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 95, "\\s1", "R to L");
		}
		if(shieldMode[1] == '0')
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 125, NULL, "L to R");
		}
		else
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 125, "\\s1", "L to R");
		}
		if(shieldMode[2] == '0')
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 155, NULL, "TOP");
		}
		else
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 155, "\\s1", "TOP");
		}
		if(shieldMode[3] == '0')
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 185, NULL, "BOTTOM");
		}
		else
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 185, "\\s1", "BOTTOM");
		}
		if(shieldMode[4] == '0')
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 215, NULL, "SPCIAL");
		}
		else
		{
			PrintDebugDataStr(opF, 0.00, endTime, 62, 215, "\\s1", "SPCIAL");
		}
		
		/*ͳһ������ʾtotal��ֵ*/
		for(cnt = 0; cnt < 6; cnt++)
		{
			PrintDebugDataInt(opF, 0.00, endTime, 402, 65 + cnt * 30, NULL, total[cnt]);
			if(total[cnt] == 0)
			{/*������0�Ϳ�ʼͳһ��ʾ0*/
				PrintDebugDataInt(opF, 0.00, endTime, 147, 65 + cnt * 30, NULL, 0);
				PrintDebugDataInt(opF, 0.00, endTime, 317, 65 + cnt * 30, NULL, 0);
				PrintDebugDataInt(opF, 0.00, endTime, 232, 65 + cnt * 30, NULL, 0);
			}
		}
		
		if(head -> time > TIME_EPS)
		{/*�����һ����Ļ���Ǵ�0.00��ʼ�ͽ�ȫ������д0 ��head -> time > 0*/
			int i, j;
			for(i = 0; i < 3; i++)
			{
				for(j = 0; j <= NUM_OF_TYPE; j++)
				{
					PrintDebugDataInt(opF, 0.00, head -> time, 147 + i * 85, 65 + j * 30, NULL, 0);
				}
			}
		}
		fprintf(opF, "\n");
		
		/*�������������ӡȫ���仯������*/
		signPtr = now = head;
		while(now != NULL)
		{
			/*�ƶ�ָ�뵽ͬ����һ����Ļ*/
			while(GetEndTime(signPtr, speed, holdTime) < now -> time + TIME_EPS)
			{
				signPtr = signPtr -> next;
			}
			
			/*����ͬ����Ļ������*/ 
			scanPtr = signPtr;
			arrset(screen, 0, NUM_OF_TYPE + 1);
			while(scanPtr != now -> next)
			{
				if(scanPtr -> type > 0 && GetEndTime(scanPtr, speed, holdTime) > now -> time)
				{
					screen[scanPtr -> type]++;
				}
				scanPtr = scanPtr -> next;
			}
			
			/*��ǰ��Ļ���뵯Ļ���������������ε�Ļ������*/
			if(now -> type > 0 && now -> type <= 5)
			{
				count[now -> type]++;
			}
			else if(now -> type < 0 && now -> type >= -5)
			{
				shield[-(now -> type)]++;
			}
			
			
			/*��ʼ���±������*/
			if(now -> next != NULL)
			{/*��ʼѰ��ʱ����������һ����Ļ�Ŀ�ʼ*/
				endTime = (now -> next) -> time;
			}
			else
			{/*���һ��Ѱ��ʱ�����������һ����Ļ��ʼʱ���ӳ�30��*/ 
				endTime = now -> time + 30;
			}
			
			if(fabs(now -> time - endTime) > TIME_EPS)
			{
				/*�������ε�Ļ�������뵯Ļ����������*/
				count[0] = count[1] + count[2] + count[3] + count[4] + count[5];
				shield[0] = shield[1] + shield[2] + shield[3] + shield[4] + shield[5];
				for(cnt = 0; cnt <= NUM_OF_TYPE; cnt++)
				{
					if(total[cnt])
					{
						PrintDebugDataInt(opF, now -> time, endTime, 232, 65 + 30 * cnt, NULL, shield[cnt]);
						PrintDebugDataInt(opF, now -> time, endTime, 317, 65 + 30 * cnt, NULL, count[cnt]);
					}
				}
				
				/*����ͬ����Ļ������ ע*������Ļ֮ǰ�����е�Ļ�˳���Ļ*/
				int sameTimeNum[6] = {0};/*��ͬ���͵�Ļͬʱ���ֵĴ���������*/
				int minTimeType = 0;/*��Сʱ���Ӧ�ĵ�Ļ����*/
				float debugTempTime, debugMinTime;
				startTime = endTime = now -> time;
				while(TRUE)
				{/*��ͬ����Ļ�в���Ѱ�ҽ���ʱ�����Сֵ�������ʱ�����Ҫ����һ��ͬ����Ļ-1������*/
					scanPtr = signPtr;
					arrset(sameTimeNum, 0, NUM_OF_TYPE + 1);
					if(now -> next == NULL)
					{/*��ʼѰ��ʱ����������һ����Ļ�Ŀ�ʼ*/
						debugMinTime = now -> time + 30;
					}
					else
					{/*���һ��Ѱ��ʱ�����������һ����Ļ��ʼʱ���ӳ�30��*/
						debugMinTime = (now -> next) -> time;
					}
					while(scanPtr != now -> next)
					{/*ѭ��Ѱ����Сʱ�� ������������¼����ʱ��*/
						if((debugTempTime = GetEndTime(scanPtr, speed, holdTime)) < TIME_EPS)
						{/*����debugTempTime����0.00��ʾ��Ļ�����˴���*/
							scanPtr = scanPtr -> next;
							continue;
						}
						
						if(debugTempTime > endTime && debugTempTime < debugMinTime &&
						   ((now->next == NULL && debugTempTime < now->time + 30) || 
						    (now->next != NULL && debugTempTime < (now->next) -> time)) )
						{/*debugMinTime > debugTempTime > endTime �� debugTempTime���޶���Χ��*/
							debugMinTime = debugTempTime;
							
							/*��Сֵ��ˢ���ˣ�֮ǰ����ͬ���͵�Ļͳ��Ҳû��������*/
							minTimeType = scanPtr -> type;
							arrset(sameTimeNum, 0, NUM_OF_TYPE + 1);
						}
						if(debugMinTime == debugTempTime)
						{
							/*ͳ��ͬһʱ����ʧ�ĸ����͵�Ļ����*/
							sameTimeNum[scanPtr -> type]++;
						}
						scanPtr = scanPtr -> next;
					}
					endTime = debugMinTime;
					if(startTime == endTime)
					{/*�Ҳ���һ����Сʱ�� �� ֵδ�ı� ��������*/
						break;
					}
					
					screen[0] = screen[1] + screen[2] + screen[3] + screen[4] + screen[5];
					for(cnt = 0; cnt <= NUM_OF_TYPE; cnt++)
					{/*д����*/
						if(total[cnt])
						{
							PrintDebugDataInt(opF, startTime, endTime,
											  147, 65 + 30 * cnt, NULL, screen[cnt]);
						}
					}
					startTime = endTime;/*�Ա�����¼�Ľ���ʱ����Ϊ��һ����¼�Ŀ�ʼʱ��*/
					for(cnt = 0; cnt <= NUM_OF_TYPE; cnt++)
					{/*����ͬ����Ļ������ ��ȥͬһʱ����ʧ�ĵ�Ļ*/ 
						screen[cnt] -= sameTimeNum[cnt];
					}
				}
			}
			
			now = now -> next;
		}/*���� while*/

	}/*���� if*/
	
	/*����ͼ�����ر�*/
	if(mode[0] != '0' || mode[1] != '0')
	{
		/*��ʾ���ֵ׿��������*/
		fprintf(opF, "\nDialogue: 9,");
		PrintTime(opF, lastDanmakuStartTime, ",");
		PrintTime(opF, lastDanmakuStartTime + 0.20, ",");
		/*�׿�*/
		if(mode[0] != '0')
		{
			fprintf(opF, "Default,,0000,0000,0000,,{\\an4\\move(20,377,20,423)"
						 "\\iclip(m 20 340 l 445 340 l 445 385 l 20 385 l 20 340)\\1c&HEAF3CE\\1a&H35\\shad0"
						 "\\bord0\\p1}m 15 0 l 410 0 b 416 0 425 9 425 15 l 425 30 b 425 36 416 45 410 45"
						 " l 15 45 b 9 45 0 37 0 30 l 0 15 b 0 9 9 0 15 0{\\p0}");
		}
		else
		{
			fprintf(opF, "Default,,0000,0000,0000,,{\\an4\\move(20,122,20,168)"
						 "\\iclip(m 20 85 l 445 85 l 445 130 l 20 130 l 20 85)\\1c&HEAF3CE\\1a&H35\\shad0"
						 "\\bord0\\p1}m 15 0 l 410 0 b 416 0 425 9 425 15 l 425 30 b 425 36 416 45 410 45"
						 " l 15 45 b 9 45 0 37 0 30 l 0 15 b 0 9 9 0 15 0{\\p0}");
		}
		
		
		
		/*��ʾ���ֵ׿�̶���*/
		fprintf(opF, "\nDialogue: 9,");
		PrintTime(opF, lastDanmakuStartTime + 0.20, ",");
		PrintTime(opF, lastDanmakuStartTime + 30.00, ",");
		/*�׿�*/
		if(mode[0] != '0')
		{
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\1c&HEAF3CE\\1a&H35\\shad0\\bord0\\p1}"
						 "m 35 400 l 430 400 b 436 400 445 409 445 415 l 445 430 b 445 436 436 445 430 445"
						 " l 35 445 b 29 445 20 437 20 430 l 20 415 b 20 409 29 400 35 400{\\p0}");
		}
		else
		{
			fprintf(opF, "Default,,0000,0000,0000,,{\\an7\\1c&HEAF3CE\\1a&H35\\shad0\\bord0\\p1}"
						 "m 35 145 l 430 145 b 436 145 445 154 445 160 l 445 175 b 445 181 436 190 430 190"
						 " l 35 190 b 29 190 20 182 20 175 l 20 160 b 20 154 29 145 35 145{\\p0}");
		}
		
		/*ˢ���˳�ʱ��*/
		for(cnt = 0; cnt < 30; cnt++)
		{
			fprintf(opF, "\nDialogue: 9,");
			PrintTime(opF, lastDanmakuStartTime + cnt, ",");
			PrintTime(opF, lastDanmakuStartTime + cnt + 1.00, ",");
			/*�׿�*/
			if(mode[0] != '0')
			{
				fprintf(opF, "Default,,0000,0000,0000,,{\\an4\\pos(35,422)\\b1\\fs25\\1c&HFFFFFF\\1a&H35"
							 "\\shad1\\bord0}These charts will close after %d s", 30 - cnt);				
			}
			else
			{
				fprintf(opF, "Default,,0000,0000,0000,,{\\an4\\pos(35,167)\\b1\\fs25\\1c&HFFFFFF\\1a&H35"
							 "\\shad1\\bord0}These charts will close after %d s", 30 - cnt);					
			}
		}
	}
	fflush(opF);
	fclose(opF);
	return 0;
}

/*
��ȡ��Ļ����ʱ�䣨�룩
������ 
��Ļָ��/������Ļ�ٶ�/�̶���Ļ����ʱ�� 
����ֵ��
����ʱ��
���󷵻�0.00 
*/
float GetEndTime(DANMAKU *danmakuPtr, const int speed, const int holdTime)
{/*[0,0.17,"1-1",7,"�ı���������",0,0,0,0.17,500,0,true,"΢���ź�",1]*/
	if(danmakuPtr == NULL)
	{
		return 0.00;
	}
	
	if(IS_R2L(danmakuPtr) || IS_L2R(danmakuPtr))
	{
		return danmakuPtr -> time + speed;
	}
	else if(IS_TOP(danmakuPtr) || IS_BTM(danmakuPtr))
	{
		return danmakuPtr -> time + holdTime;
	}
	else if(IS_SPECIAL(danmakuPtr))
	{/*���ⵯĻ��Ҫȡ����ʱ��*/ 
		char textTemp[64], *textPtr = danmakuPtr -> text;
		int cnt;
		for(cnt = 0; cnt < 3; cnt++)
		{/*ֱ��������������*/
			while(*textPtr != ',')
			{
				textPtr++;
			}
			textPtr++;
		}
		return danmakuPtr -> time + StrToFloat(DeQuotMarks(StrGetLeftPart(textTemp, &textPtr, ',')));
	}
	else
	{
		return 0.00;
	}
	
}

/*
��ass��ʽ��׼��ӡʱ�� 
������ 
�ļ�ָ��/����/�����Ŵ�ӡ�ں�����ַ���
����ֵ��
ferror�����ķ���ֵ 
*/
int PrintTime(FILE *filePtr, float time, char *endText)
{
	int hour = (int)time / 3600;
	time -= hour * 3600;
	int min = (int)time / 60;
	time -= min * 60;
	int sec = (int)time;
	int ms = (time - sec) * 100;
	fprintf(filePtr, "%01d:%02d:%02d.%02d%s", hour, min, sec, ms, endText);
	return ferror(filePtr);
}

/*
ת��ʮ����rgb��ɫΪʮ��������ɫ
������
ʮ������ɫ/��¼ʮ�����Ƶ����飨7���ֽ����ϣ� 
����ֵ��
ʮ����������ĵ�ַ
*/ 
char *ToHexColor(int decColor, char *hexColor)
{/*��ת��Ϊ16����������Ϊһ�Ե�������*/
	int i, j;
	/*ʹ������forʵ��λ�ý���*/ 
	for(i = 0; i < 3; i++)
	{
		for(j = 1; j >= 0; j--)
		{
			switch(decColor % 16)
			{
				default:
				{
					hexColor[2 * i + j] = decColor % 16 + '0';
					break;
				}
				case 10:
				{
					hexColor[2 * i + j] = 'A';
					break;
				}
				case 11:
				{
					hexColor[2 * i + j] = 'B';
					break;
				}
				case 12:
				{
					hexColor[2 * i + j] = 'C';
					break;
				}
				case 13:
				{
					hexColor[2 * i + j] = 'D';
					break;
				}
				case 14:
				{
					hexColor[2 * i + j] = 'E';
					break;
				}
				case 15:
				{
					hexColor[2 * i + j] = 'F';
					break;
				}
			}
			decColor /= 16;
		}
	}
	hexColor[6] = '\0'; 
	return hexColor;
}

/*
ʮ����͸����תʮ������͸���ȣ�0-255�� 
*/
char *ToHexOpacity(int decOpacity, char *hexOpacity)
{
	int cnt;
	for(cnt = 1; cnt >=0; cnt--)
	{
		switch(decOpacity % 16)
		{
			default:
			{
				hexOpacity[cnt] = decOpacity % 16 + '0';
				break;
			}
			case 10:
			{
				hexOpacity[cnt] = 'A';
				break;
			}
			case 11:
			{
				hexOpacity[cnt] = 'B';
				break;
			}
			case 12:
			{
				hexOpacity[cnt] = 'C';
				break;
			}
			case 13:
			{
				hexOpacity[cnt] = 'D';
				break;
			}
			case 14:
			{
				hexOpacity[cnt] = 'E';
				break;
			}
			case 15:
			{
				hexOpacity[cnt] = 'F';
				break;
			}
		}
		decOpacity /= 16;
	}
	hexOpacity[2] = '\0';
	return hexOpacity;
}

/*
Ѱ����Сֵ 
������
������Сֵ������/��Ա��/��ֹ�±�/ģʽ��0����1����
����ֵ��
��Сֵ�����±� 
*/
int FindMin(float *array, const int numOfLine, const int stopSubScript, const int mode)
{
	int cnt, minSub;
	if(!mode)
	{	/*��������*/
		minSub = 0;
		for(cnt = 0; cnt < stopSubScript; cnt++)
		{
			if(array[cnt] < array[minSub])
			{
				minSub = cnt;
			}
		}
	}
	else
	{	/*��������*/ 
		minSub = numOfLine - 1;
		for(cnt = numOfLine - 1; cnt >= stopSubScript; cnt--)
		{
			if(array[cnt] < array[minSub])
			{
				minSub = cnt;
			}
		}
	}
	return minSub;
}


/*
������Ϣ��ӡһ�����Σ���ͼ���֣�
������
�ļ�ָ��/���X����/���Y����/���/�߶�
����ֵ��
ferror��������ֵ 
*/
int DrawRectangle(FILE *opF, int startX, int startY, int width, int height)
{
	fprintf(opF, "m %d %d l %d %d l %d %d l %d %d l %d %d",
				startX, startY, startX + width, startY,
				startX + width, startY + height, startX, startY + height,
				startX, startY);
	return ferror(opF);
}


/*
������Ϣ��ӡdebug���ݱ��ϵ���Ϣ 
������ 
�ļ�ָ��/��ʼ����/��������/ǿ�ƶ�λx/ǿ�ƶ�λY/��Ч׷��/��������
����ֵ��
ferror�����ķ���ֵ 
*/
int PrintDebugDataInt(FILE *filePtr, const float startTime, const float endTime, const int posX, const int posY,
					  char *effect, const int data)
{
	fprintf(filePtr, "\nDialogue:10,");
	PrintTime(filePtr, startTime, ","); 
	PrintTime(filePtr, endTime, ",");
	fprintf(filePtr, "Default,,0000,0000,0000,,{\\an5\\pos(%d,%d)\\b1", posX, posY);
	if(effect != NULL)
	{
		fprintf(filePtr, "%s", effect);
	}
	fprintf(filePtr, "\\fs25\\1c&HFFFFFF\\1a&H35\\shad1\\bord0}%d", data);
	return ferror(filePtr);
}

/*
������Ϣ��ӡdebug���ݱ��ϵ���Ϣ 
������ 
�ļ�ָ��/��ʼ����/��������/ǿ�ƶ�λx/ǿ�ƶ�λY/��Ч׷��/�ַ��� 
����ֵ��
ferror�����ķ���ֵ 
*/
int PrintDebugDataStr(FILE *filePtr, const float startTime, const float endTime, const int posX, const int posY,
					  char *effect, char *str)
{
	fprintf(filePtr, "\nDialogue:10,");
	PrintTime(filePtr, startTime, ","); 
	PrintTime(filePtr, endTime, ",");
	fprintf(filePtr, "Default,,0000,0000,0000,,{\\an5\\pos(%d,%d)\\b1", posX, posY);
	if(effect != NULL)
	{
		fprintf(filePtr, "%s", effect);
	}
	fprintf(filePtr, "\\fs25\\1c&HFFFFFF\\1a&H35\\shad1\\bord0}%s", str);
	return ferror(filePtr);
}

/*
�������� 
������ 
�����׵�ַ/Ҫ��ʼ���ɵ�ֵ/��Ա����/ 
����ֵ��
ferror�����ķ���ֵ 
*/
void arrset(int *array, const int value, const int numberOfMember)
{
	int cnt;
	for(cnt = 0; cnt < numberOfMember; cnt++)
	{
		*(array + cnt) = value;
	}
	return;
}
