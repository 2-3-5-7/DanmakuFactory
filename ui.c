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
��ʾ�����ع�� 
*/
void showCursor(BOOL show)
{
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	if (show == TRUE)
	{
		cursor.bVisible = TRUE;
	}
	else
	{
		cursor.bVisible = FALSE;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

/*
�趨���λ��
*/
void setPos(int x, int y)
{
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

/*
�Զ�����ɫ�ĸ�ʽ��� 
������ȫ����ɫ/�趨��ɫ/��ʽ�����ַ���/��Ӧ���� 
����ֵ������ֽ��� 
*/
int stylePrintf(WORD gobalStyle, WORD printStyle, char *format, ...)
{
	int cnt;	
	va_list arg;
	va_start(arg, format);
	/*�޸Ŀ���̨��ɫ*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), printStyle);
	cnt = vprintf(format, arg);
	va_end(arg);
	/*�޸Ļ�ԭ������ɫ*/ 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), gobalStyle);
	return cnt;
}

/*
��ӡ�˵����� 
��������ʼ����x/��ʼ����y/ѡ����/ѡ����ָ�����/ȫ����ɫ/ѡ������ɫ/�Ƿ�ÿ���趨��ʽ/����/��������
˵�����ɱ����ֵΪ""/��ʾ��������ת����һ�в���selected��0��ʼ�ұ������в�����
	  ���ÿ�ж��趨��ʽ��ɱ������ÿһ���������涼����һ��WORD���͵ı�ʾ��ɫ�Ĳ��� 
*/
void printMenuText(int x, int y, int selected, char *pointer,
				   WORD gobalStyle, WORD selectedStyle, BOOL allLineSetStyle,
				   int numOfLines, ...)
{
	int cnt, skip = 0;
	char *textOfLine;
	char blank[16] = {0}; 
	va_list arg;
	va_start(arg, numOfLines);
	
	for (cnt=0; cnt<strlen(pointer) && cnt < 16; cnt++)
	{
		blank[cnt] = ' ';
	}
	
	for (cnt=0; cnt<numOfLines; cnt++)
	{
		textOfLine = va_arg(arg, char *);/*���ɱ��������*/
		setPos(x, y);/*��λ*/
		
		if (textOfLine[0] == '\0')
		{/*����*/
			skip++;
		}
		else if (cnt + skip == selected)
		{/*��ѡ����*/
			if (selectedStyle != 0x00)
			{
				if (allLineSetStyle == TRUE)
				{
					va_arg(arg, int);/*����һ��������ɫ�Ĳ���*/
				}
				stylePrintf(gobalStyle, selectedStyle, "%s%s", pointer, textOfLine);
			}
			else if (allLineSetStyle == TRUE)
			{
				stylePrintf(gobalStyle, (WORD)(va_arg(arg, int)), "%s%s", pointer, textOfLine);
			}
			else
			{
				stylePrintf(gobalStyle, gobalStyle, "%s%s", pointer, textOfLine);
			}	
		}
		else if (allLineSetStyle == TRUE)
		{/*��ͨ�� ������ÿһ�е���ɫ*/
			stylePrintf(gobalStyle, (WORD)(va_arg(arg, int)), "%s%s", blank, textOfLine);
		}
		else
		{/*��ͨ�� ������ÿһ�е���ɫ*/ 
			stylePrintf(gobalStyle, gobalStyle, "%s%s", blank, textOfLine);
		}
		y++;/*�ƶ��й��*/
	}
	va_end(arg);
}

/*
�����û�����һ���ض���Χ������
������
��Χ/�ҷ�Χ/��ʽ�����ַ���/��Ӧ���� 
����ֵ��
�õ���ֵ 
*/
int inputInt(int rangeLeft, int rangeRight, char *msgFormat, ...)
{
	char tempText[MAX_TEXT_LENGTH];
	int getNum;
	va_list arg;
	va_start(arg, msgFormat);
	do {
		fflush(stdin);
		vprintf(msgFormat, arg);
		fgets(tempText, MAX_TEXT_LENGTH, stdin);
		if(tempText[strlen(tempText) - 1] == '\n')
		{/*�Ȼ�ȡ�ַ����ж��Ƿ��зǷ��ַ�*/
			tempText[strlen(tempText) - 1] = '\0';
		}
		getNum = strToInt(tempText);
	} while (getNum < rangeLeft || getNum > rangeRight || !isDesignatedChar(tempText, "-0123456789"));
	
	va_end(arg);
	return getNum;
}
