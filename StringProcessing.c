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
�����ַ����߶� 
������
�ַ���ָ��/�ֺ�/���� 
����ֵ��
�߶ȣ����أ� 
*/
int getStrHei(unsigned char *str, const int fontSizeSet, const int fontSizeInFile, const char *fontName)
{
	return fontSizeSet + (fontSizeInFile - 25);
}

/*
�����ַ������
������
�ַ���ָ��/�ֺ�/����
����ֵ��
��ȣ����أ� 
*/
int getStrLen(unsigned char *str, const int fontSizeSet, const int fontSizeInFile, const char *fontName)
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
	while (*str != '\0')
	{
		if (*str >= 0xC0)
		{
			cnt += 2;
		}
		else if (*str < 0x80)
		{
			cnt += 1;
		}
		str++;
	}
	return (fontSizeSet + (fontSizeInFile - 25)) / 2 * cnt;
}

/*
�ַ�����ʮ���ƣ�ת��Ϊ������
������������Ž�����
*/
float strToFloat(char *str)
{
	float sum = 0;
	int sign = 1;
	if (*str == '(')
	{
		str++;
	}
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str != '.')
	{
		if (*str == '\0' || *str == ')')
		{
			return sum * sign;
		}
		sum = sum * 10 + (*str - '0');
		str++;
	}
	str++;
	int cnt = 1;
	while (*str != '\0' && *str != ')')
	{
		sum += pow(0.1, cnt) * (*str - '0');
		cnt++;
		str++;
	}
	return sum * sign;
}

/*
�ַ�����ʮ���ƣ�ת��Ϊ����
������������Ž�����
*/
int strToInt(char *str)
{
	int sum = 0, sign = 1;
	if (*str == '(')
	{
		str++;
	}
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str != '\0' && *str != ')')
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
char *strGetLeftPart(char *opStr, char **ipStr, char cutCh)
{
	char *rtStr = opStr;
	while (**ipStr != '\0' && **ipStr != cutCh)
	{
		*opStr = **ipStr;
		opStr++;
		(*ipStr)++;
	}
	if (**ipStr == cutCh)
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
char *deQuotMarks(char *str)
{
	char *rtStr = str;
	/*ȥǰ����*/
	if (*str == '\"')
	{/*�ж��ǲ���ֱ�ӵ�����*/ 
		while(*(str + 1) != '\0')
		{
			*str = *(str + 1);
			str++;
			
		}
	}
	else if (*str == '&' && *(str + 1) == 'q' && *(str + 2) == 'u' &&
			*(str + 3) == 'o' && *(str + 4) == 't' && *(str + 5) == ';')
	{/*�ж��ǲ������ŵ�ת���ַ�*/ 
		while (*(str + 6) != '\0')
		{
			*str = *(str + 6);
			str++;
		}
	}
	else
	{
		while (*str != '\0')
		{
			str++;
		}
	}
	
	/*ȥ������*/
	if (*(str - 1) == '\"')
	{
		*(str - 1) = '\0';
	}
	else if (*(str - 6) == '&' && *(str - 5) == 'q' && *(str - 4) == 'u' &&
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

/*
ȥ�ַ����������ţ�û�еĻ��򲻴���
����ֵ�������ָ�� 
*/
char *deBrackets(char *str)
{
	char *rtStr = str;
	/*ȥǰ����*/
	if (*str == '(')
	{
		while (*(str + 1) != '\0')
		{
			*str = *(str + 1);
			str++;
		}
	}
	else
	{
		while (*str != '\0')
		{
			str++;
		}
	}
	
	/*ȥ������*/
	if (*(str - 1) == ')')
	{
		*(str - 1) = '\0';
	}
	else
	{
		*str = '\0';
	}
	
	return rtStr;
}

/*
��ȥָ��λ�����ҿո�
������
Ҫȥ�ո���ַ���/ָ����λ�õ�ַ/
����ֵ��
����ַ���/
Ҫȥָ�����ַ����ҿո����strchr()������� 
*/
char *deDesignatedSpace(char *const str, char *startPtr)
{
	/*�Ƿ����*/
	if (str == NULL || startPtr == NULL)
	{
		return NULL;
	}
	
	char *leftPtr, *rightPtr;
	
	/*ȥ��ߵĿո�*/
	leftPtr = startPtr - 1;
	rightPtr = startPtr;
	while (leftPtr >= str)
	{
		if (*leftPtr != ' ')
		{
			break;
		}
		leftPtr--;
	}
	
	if (leftPtr < startPtr - 1)
	{
		leftPtr++;/*�����һλ�Ƶ��ո��λ��*/
		startPtr = leftPtr;
		while (*rightPtr != '\0')
		{
			*leftPtr = *rightPtr;
			leftPtr++;
			rightPtr++;
		}
		
		*leftPtr = '\0';
	}
	
	/*ȥ�ұߵĿո�*/
	leftPtr = rightPtr = startPtr + 1;
	while (*rightPtr != '\0')
	{
		if (*rightPtr != ' ')
		{
			break;
		}
		rightPtr++;
	}
	
	while (*rightPtr != '\0')
	{
		*leftPtr = *rightPtr;
		leftPtr++;
		rightPtr++;
	}
	*leftPtr = '\0';
	return str; 
}

/*
��ȥ�ַ������ҿո� 
������
Ҫȥ�ո���ַ���/
����ֵ��
����ַ���/ 
*/
char *deLeftRightSpace(char *const str)
{
	/*�Ƿ����*/
	if (str == NULL)
	{
		return NULL;
	}
	
	int total = 0;
	char *leftPtr, *rightPtr;
	leftPtr = rightPtr = str;
	/*ȥ��߿ո�*/
	while (*rightPtr == ' ')
	{
		rightPtr++;
	}
	
	while (*rightPtr != '\0')
	{
		*leftPtr = *rightPtr;
		leftPtr++;
		rightPtr++;
	}
	
	/*ȥ�ұ߿ո�*/
	leftPtr--;
	while (*leftPtr == ' ')
	{
		leftPtr--;
	}
	*(leftPtr + 1) = '\0';
	
	return str; 
}

/*
ת����Сд��ĸ
������
����ַ�����ַ/�����ַ�����ַ/ 
����ֵ��
����ַ�����ַ
������ַ�����ַΪNULL����ֱ���޸������ַ���
*/
char *toLower(char *output, char *input)
{
	char *ptr;
	if (output == NULL)
	{
		ptr = input;
	}
	else
	{
		strcpy(output, input);
		ptr = output;
	}
	
	while (*ptr != '\0')
	{
		if (*ptr >= 'A' && *ptr <= 'Z')
		{
			*ptr += 'a' - 'A';
		}
		ptr++;
	}
	
	if (output == NULL)
	{
		return input;
	}
	return output;
}

/*
�ж��ַ������ַ��Ƿ���ָ���ַ�
�����ַ���/����ָ���ַ����ַ���/ 
*/
BOOL isDesignatedChar(char *str, char *designatedChar)
{
	int cnt = 0;
	int i, j;
	for (i = 0; i < strlen(str); i++)
	{
		for (j = 0; j < strlen(designatedChar); j++)
		{
			if (str[i] == designatedChar[j])
			{
				cnt++;
			}
		}
	}
	
	if (cnt == strlen(str))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
����WINAPI���б����໥ת��
������
ԭ�ַ�������/Ŀ���ַ�������/�����ַ���/����ַ���/���������С��0Ϊ����飩/ 
����ֵ��
����ַ���/
֧���κ�ϵͳ֧�ֵı��� 
opStr Ϊ NULL �򽫽�����ص� ipStr �У�����ֵΪ�����ַ���
*/
char *transcoding(UINT sourceCode, UINT targetCoding, char *const ipStr, char *const opStr, int sizeOfOp)
{
	/*�Ƿ����*/
	if (ipStr == NULL)
	{
		return NULL;
	}
	
	int len;
	wchar_t *unicode;
	char *targetStr;
	/*תunicode*/
	len = MultiByteToWideChar(sourceCode, 0, ipStr, -1, NULL, 0);
	if ((unicode = malloc(len * sizeof(wchar_t))) == NULL)
	{
		return NULL;
	}
	
	MultiByteToWideChar(sourceCode, 0, ipStr, -1, unicode, len);
	
	/*תĿ�����*/
	len = WideCharToMultiByte(targetCoding, 0, unicode, -1, NULL, 0, NULL, NULL);
	if (sizeOfOp < len && sizeOfOp != 0)/*�ж���������Ƿ��㹻��*/
	{
		free(unicode);
		return NULL;
	}
	if ((targetStr = malloc(len * sizeof(char))) == NULL)
	{
		return NULL;
	}
	WideCharToMultiByte(targetCoding, 0, unicode, -1, targetStr, len, NULL, NULL);
	
	/*���տռ䷵�ؽ��*/
	if (opStr != NULL)
	{
		strcpy(opStr, targetStr);
		free(unicode);
		free(targetStr);
		return opStr;
	}
	else
	{
		strcpy(ipStr, targetStr);
		free(unicode);
		free(targetStr);
		return ipStr;
	}
}

/*
�ж�һ���ַ����Ƿ���utf-8
����ַ����Ƿ���ȫ����utf-8����淶 
*/
BOOL isUtf8(char const *const str)
{	
	/*�Ƿ����*/
	if (str == NULL)
	{
		return FALSE;
	}
	
	int i, len;
    const unsigned char *ptr = str;
    while ((*ptr) != '\0')
    {
    	if (((*ptr) & 0x80) == 0x00)
    	{//0000 0000 - 0000 007F	0xxxxxxx
    		ptr++;
    		continue;
		}
    	else if (((*ptr) & 0xE0) == 0xC0)
    	{//0000 0080 - 0000 07FF	110xxxxx 10xxxxxx
    		len = 1;
		}
		else if (((*ptr) & 0xF0) == 0xE0)
    	{//0000 0800 - 0000 FFFF	1110xxxx 10xxxxxx 10xxxxxx
    		len = 2;
		}
		else if (((*ptr) & 0xF8) == 0xF0)
    	{//0001 0000 - 0010 FFFF	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    		len = 3;
		}
		else if (((*ptr) & 0xFC) == 0xF8)
    	{//0020 0000 - 03FF FFFF	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    		len = 4;
		}
		else if (((*ptr) & 0xFE) == 0xFC)
    	{//0400 0000 - 7FFF FFFF	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    		len = 5;
		}
		else
		{
			return FALSE;
		}
    	
    	for (i = 0; i < len; i++)
		{
			ptr++;
			if (*ptr == '\0' || ((*ptr) & 0xC0) != 0x80)
			{
				return FALSE;
			}
		}
		ptr++;
	}
	
	return TRUE;
}
