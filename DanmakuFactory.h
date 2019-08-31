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

#ifndef __DANMAKU_FACTORY_H__
#define __DANMAKU_FACTORY_H__

#include <windows.h>
#include <conio.h>
#include <assert.h>
#include <time.h>
#include <io.h>
#include <direct.h> 
#include <stdarg.h>

#define VERSION "1.31 release"

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13

#define MAX_TEXT_LENGTH 1024

/*�������� ���ڶ�ȡ�������ļ�*/
typedef struct OldConfig
{
	float timeShift;
	int resX, resY, fontSize, shadow, outline, rollTime, holdTime, density, opacity, blank;
	char fontName[MAX_TEXT_LENGTH];
	char blockMode[8];
	char debugMode[3];
} OLDCONFIG;

/*1.30�汾����*/ 
typedef struct taskConfig
{
	/*��д�������ļ�����*/
	char input[MAX_TEXT_LENGTH];
//	char inputFormat[MAX_TEXT_LENGTH];
	char output[MAX_TEXT_LENGTH];
//	char outputFormat[MAX_TEXT_LENGTH];
	float timeShift;
	
	/*д�������ļ�����*/ 
//	UINT inputEncoding;/*�����ļ�����*/
	UINT outputEncoding;/*����ļ�����*/ 
	int resX, resY, fontSize, shadow, outline, rollTime, holdTime, density, opacity, blank;
	char fontName[MAX_TEXT_LENGTH];
	char blockMode[8];
	char debugMode[3];
} CONFIG;

/*main.c*/
void readConfigFile(CONFIG *config, char *fileName);/*�������ļ�*/
BOOL writeConfigFile(CONFIG config, char *fileName);/*д�����ļ�*/
void printErrInfo(char *code);/*��ӡ������Ϣ*/ 

/*ui.c*/
void showCursor(BOOL show);/*��ʾ�����ع��*/
void setPos(int x, int y);/*�ƶ�ָ��*/
int stylePrintf(WORD gobalStyle, WORD printStyle, char *format, ...);/*�Զ�����ɫ�ĸ�ʽ���*/
void printMenuText(int x, int y, int selected, char *pointer,
				   WORD gobalStyle, WORD selectedStyle, BOOL allLineSetStyle,
				   int numOfLines, ...);/*��ӡ�˵�����*/
int inputInt(int rangeLeft, int rangeRight, char *msgFormat, ...);/*�����û�����һ���ض���Χ������*/ 
 
#endif
