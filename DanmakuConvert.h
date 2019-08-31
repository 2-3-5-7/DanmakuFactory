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

#ifndef __DANMAKU_CONVERT_H__
#define __DANMAKU_CONVERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_OF_TYPE 5
#define FLOAT_EPS 1e-4 
#define TIME_EPS 1e-4
#define MAX_TEXT_LENGTH 1024

#define TRUE 1
#define FALSE 0

#define ANSI CP_ACP
#define UTF_8 CP_UTF8

#define PRINT_ERR FALSE

#define IS_R2L(ptr) (ptr -> type == 1 || ptr -> type == -1)
#define IS_L2R(ptr) (ptr -> type == 2 || ptr -> type == -2)
#define IS_TOP(ptr) (ptr -> type == 3 || ptr -> type == -3)
#define IS_BTM(ptr) (ptr -> type == 4 || ptr -> type == -4)
#define IS_SPECIAL(ptr) (ptr -> type == 5 || ptr -> type == -5)
#define IS_NORMAL(ptr) (IS_R2L(ptr) || IS_L2R(ptr) || IS_TOP(ptr) || IS_BTM(ptr) || IS_SPECIAL(ptr))

typedef int BOOL;

typedef struct Danmaku
{
	float time;
	short type;
	short fontSize;
	int color;
	char *text;
	struct Danmaku *next;
} DANMAKU;

/*ReadFile.c*/
int readXml(const char *ipFile, DANMAKU **head, const char *mode, const float timeShift);/*��ȡxml�ļ����뵯Ļ��*/
void static errorExit(FILE *ipF, DANMAKU *head, DANMAKU *ptr);/*���������˳�ǰ�Ĵ���*/

/*WriteFile.c*/
int writeAss(const char *opFileName, DANMAKU *head, const int resX, int resY, const int fontSize,
			 char *fontName,const int shadow, const int outline, const float rollTime, const float holdTime, 
			 const int density, const int opacity, const int blank,
			 char *blockMode, char *debugMode);/*����Ļ��д��ass�ļ�*/
int writeAssDebugPath(const char *opFileName, DANMAKU *head, char *mode, const int rollTime, const int holdTime,
					  char *blockMode);/*дass��Ļ��debug����*/
float static getEndTime(DANMAKU *danmakuPtr, const int rollTime, const int holdTime);/*��ȡ��Ļ����ʱ��*/
int static printTime(FILE *filePtr, float time, char *endText);/*��ass��ʽ��׼��ӡʱ��*/
char static *toHexColor(int decColor, char *hexColor);/*ת��ʮ����rgb��ɫΪʮ��������ɫ*/
char static *toHexOpacity(int decOpacity, char *hexOpacity);/*ʮ����͸����תʮ������͸����*/
int static findMin(float *array, const int numOfLine, const int stopSubScript, const int mode);/*Ѱ����Сֵ*/
int static drawRectangle(FILE *opF, int startX, int startY, int width, int height);/*������Ϣ��ӡһ��ass����*/
int static printDebugDataInt(FILE *filePtr, const float startTime, const float endTime, const int posX,
								const int posY, char *effect, const int data);/*������Ϣ��ӡdebug���ݱ��ϵ���Ϣ*/
int static printDebugDataStr(FILE *filePtr, const float startTime, const float endTime, const int posX,
						const int posY, char *effect, char *str);/*������Ϣ��ӡdebug���ݱ��ϵ���Ϣ*/
void static arrset(int *array, const int value, const int numberOfMember);/*�������*/

/*StringProcessing.c*/
int getStrHei(unsigned char *str, const int fontSizeSet,
			  const int fontSizeInFile, const char *fontName);/*�����ַ����߶�*/
int getStrLen(unsigned char *str, const int fontSizeSet,
			  const int fontSizeInFile, const char *fontName);/*�����ַ������*/
float strToFloat(char *str);/*�ַ�����ʮ���ƣ�ת��Ϊ������*/
int strToInt(char *str);/*�ַ�����ʮ���ƣ�ת��Ϊ����*/
char *strGetLeftPart(char *opStr, char **ipStr, char cutCh);/*�õ��ַ�����߲��֣����ƶ�ָ�����ض�λ�ú�һ���ַ�*/
char *deQuotMarks(char *str);/*ȥ�ַ����������ţ�û�еĻ��򲻴���*/
char *deBrackets(char *str);/*ȥ�ַ����������ţ�û�еĻ��򲻴���*/
char *deDesignatedSpace(char *const str, char *startPtr);/*��ȥָ��λ�����ҿո�*/
char *deLeftRightSpace(char *const str);/*��ȥ�ַ������ҿո�*/
char *toLower(char *output, char *input);/*ת����Сд��ĸ*/
BOOL isDesignatedChar(char *str, char *designatedChar);/*�ж��ַ������ַ��Ƿ���ָ���ַ�*/
char *transcoding(UINT sourceCode, UINT targetCoding,
				  char *const ipStr, char *const opStr, int sizeOfOp);/*����WINAPI���б����໥ת��*/
BOOL isUtf8(char const *const str);/*�ж�һ���ַ����Ƿ���utf-8*/

/*ListProcessing.c*/
int sortList(DANMAKU **listHead);/*������������Ͱ����*/
void freeList(DANMAKU *listHead);/*�ͷ���������*/
void transListCoding(DANMAKU *listHead, UINT targetCoding);/*ת��������Ļ�ص��ı�����*/

#endif
