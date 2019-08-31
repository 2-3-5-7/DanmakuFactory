#include "DanmakuFactory.h"

/*
�����ַ����߶� 
������
�ַ���ָ��/�û��趨���ִ�С/ԭ��Ļ�ļ������ִ�С/���� 
����ֵ��
�߶ȣ����أ� 
*/
int GetStrHei(unsigned char *str, const int fontSizeSet, const int fontSizeInFile, const char *fontName)
{
	return fontSizeSet + (fontSizeInFile - 25);
}

/*
�����ַ������
������
�ַ���ָ��/�û��趨���ִ�С/ԭ��Ļ�ļ������ִ�С/���� 
����ֵ��
��ȣ����أ� 
*/
int GetStrLen(unsigned char *str, const int fontSizeSet, const int fontSizeInFile, const char *fontName)
{
/*
Ŀǰ�����utf-8 
���ֽ�			�ռ�ռ��		��ʼ 
0--- ---- 		1 byte			0x00 
110- ----		2 byte			0xC0 
1110 ----		3 byte			0xE0
1111 0---		4 byte			0xF0
1111 10--		5 byte			0xF8 
1111 110-		6 byte			0xFC
*/
	int cnt = 0;
	while(*str != '\0')
	{
		if(*str >= 0xC0)
		{
			cnt += 2;
		}
		else if(*str < 0x80)
		{
			cnt += 1;
		}
		str++;
	}
	return (fontSizeSet + (fontSizeInFile - 25)) / 2 * cnt;
}

/*
�ַ�����ʮ���ƣ�ת��Ϊ������
*/
float StrToFloat(char *str)
{
	float sum = 0;
	int sign = 1;
	if(*str == '-')
	{
		sign = -1;
		str++;
	}
	while(*str != '.')
	{
		if(*str == '\0')
		{
			return sum * sign;
		}
		sum = sum * 10 + (*str - '0');
		str++;
	}
	str++;
	int cnt = 1;
	while(*str != '\0')
	{
		sum += pow(0.1, cnt) * (*str - '0');
		cnt++;
		str++;
	}
	return sum * sign;
}

/*
�ַ�����ʮ���ƣ�ת��Ϊ����
*/
int StrToInt(char *str)
{
	int sum = 0, sign = 1;
	if(*str == '-')
	{
		sign = -1;
		str++;
	}
	while(*str != '\0')
	{
		sum = sum * 10 + *str - '0';
		str++;
	}
	return sum * sign;
}

/*
�õ��ַ�����߲��֣����ƶ�ָ�����ض�λ�ú�һ���ַ� 
����������ַ���/�����ַ���ָ���ַ/�ض�λ��/ 
����ֵ������ַ�����ַ 
*/
char *StrGetLeftPart(char *opStr, char **ipStr, char cutCh)
{
	char *rtStr = opStr;
	while(**ipStr != '\0' && **ipStr != cutCh)
	{
		*opStr = **ipStr;
		opStr++;
		(*ipStr)++;
	}
	if(**ipStr == cutCh)
	{
		(*ipStr)++;
	}
	*opStr = '\0';
	return rtStr;
}

/*
ȥ�ַ����������ţ�û�еĻ��򲻴���
����ֵ�������ָ�� 
*/
char *DeQuotMarks(char *str)
{
	char *rtStr = str;
	/*ȥǰ����*/
	if(*str == '\"')
	{/*�ж��ǲ���ֱ�ӵ�����*/ 
		while(*(str + 1) != '\0')
		{
			*str = *(str + 1);
			str++;
			
		}
	}
	else if(*str == '&' && *(str + 1) == 'q' && *(str + 2) == 'u' &&
			*(str + 3) == 'o' && *(str + 4) == 't' && *(str + 5) == ';')
	{/*�ж��ǲ������ŵ�ת���ַ�*/ 
		while(*(str + 6) != '\0')
		{
			*str = *(str + 6);
			str++;
		}
	}
	else
	{
		while(*str != '\0')
		{
			str++;
		}
	}
	
	/*ȥ������*/
	if(*(str - 1) == '\"')
	{
		*(str - 1) = '\0';
	}
	else if(*(str - 6) == '&' && *(str - 5) == 'q' && *(str - 4) == 'u' &&
			*(str - 3) == 'o' && *(str - 2) == 't' && *(str - 1) == ';')
	{
		*(str - 6) = '\0';
	}
	else
	{
		*str = '\0';
	}
	
	return rtStr;
}
