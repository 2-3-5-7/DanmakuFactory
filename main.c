#include "DanmakuFactory.h"
#include <io.h>

typedef struct Config
{
	float timeShift;
	int resX, resY, fontSize, shadow, outline, speed, holdTime, density, opacity, blank;
	char fontName[MAX_TEXT_LENGTH], shieldMode[8], debugMode[3];
} CONFIG;

void PrintFunctionDebugData(char *name, long wasteTime, int returnValue);
char *AddSlash(char *ipStr);

int main(void)
{
	char inputFileName[MAX_TEXT_LENGTH], outputFileName[MAX_TEXT_LENGTH], displayFileName[MAX_TEXT_LENGTH];
	/*�ļ�����ҳ*/ 
	printf("DanmakuFactory v%s\n", VERSION);
	printf("------------------------\n");
	/*��д��������ļ�·��*/
	while(TRUE)
	{
		fflush(stdin);
		printf("�����ļ��� ");
		fgets(displayFileName, MAX_TEXT_LENGTH / 2, stdin);
		if(displayFileName[strlen(displayFileName + 1)] == '\n')
		{
			displayFileName[strlen(displayFileName + 1)] = '\0';
		}
		DeQuotMarks(displayFileName);/*����еĻ� ȥ��������*/
		if(access(displayFileName, 0))
		{
			printf("�ļ� %s �����ڣ�����������\n", displayFileName); 
		}
		else
		{
			break;
		}
	}
	
	strcpy(inputFileName, displayFileName);
	AddSlash(inputFileName);/*��һ��б��ת��������б��*/
	strcpy(outputFileName, inputFileName);
	strcat(outputFileName, ".ass");
	fflush(stdin);
	
	
	if(!access(outputFileName, 0))
	{
		printf("\n����ļ�%s.ass�Ѵ��ڣ�����ִ��ԭ�ļ����ᱻ����\n", displayFileName);
		printf("����y��Y���������������ַ��˳�\n");
		char ch = getchar();
		if(ch != 'Y' && ch != 'y')
		{
			exit(0);
		}
	}
	
	
	/*��ȡ�����ļ�*/
	FILE *configFile;
	CONFIG danmakuConfig;
	if((configFile = fopen("DanmakuFactoryConfig.bin", "rb")) == NULL)
	{
		danmakuConfig.resX = 1920;
		danmakuConfig.resY = 1080;
		
		danmakuConfig.speed = 10;
		danmakuConfig.holdTime = 5;
		danmakuConfig.density = 0;
		
		danmakuConfig.opacity = 180;
		danmakuConfig.outline = 0;
		danmakuConfig.shadow = 0;
		strcpy(danmakuConfig.fontName, "Microsoft YaHei Light");
		
		danmakuConfig.timeShift = 0.00;
		danmakuConfig.blank = 0; 
		strcpy(danmakuConfig.shieldMode, "0000000");
		strcpy(danmakuConfig.debugMode, "00");
		danmakuConfig.fontSize = 38;
	}
	else
	{
		fread(&danmakuConfig, sizeof(CONFIG), 1, configFile);
		fclose(configFile);
		configFile = NULL;
	}
	
	/*��Ļ����ҳ*/
	while(TRUE)
	{
		system("cls");
		printf("DanmakuFactory v%s\n", VERSION);
		printf("%s\n\n", displayFileName);
		printf("��Ļ����\n");
		printf("------------------------");
		printf("\n 1 �ֱ��ʿ� = %d", danmakuConfig.resX);
		printf("\n 2 �ֱ��ʸ� = %d", danmakuConfig.resY);
		
		printf("\n\n 3 ������Ļ�ٶȣ��룩 = %d", danmakuConfig.speed);
		printf("\n 4 �̶���Ļ�ٶȣ��룩 = %d", danmakuConfig.holdTime);
		printf("\n 5 ��Ļ�ܶȣ����� = %d", danmakuConfig.density);
		
		printf("\n\n 6 ���ִ�С = %d", danmakuConfig.fontSize);
		printf("\n 7 ���� = %s", danmakuConfig.fontName);
		printf("\n 8 ��͸���ȣ�0-255�� = %d", danmakuConfig.opacity);
		printf("\n 9 ��ߣ�0-4�� = %d", danmakuConfig.outline);
		printf("\n a ��Ӱ��0-4�� = %d", danmakuConfig.shadow);
		
		printf("\n\n b ʱ��ƫ�ƣ��������� = %f", danmakuConfig.timeShift);
		printf("\n c �·����ף����أ� = %d", danmakuConfig.blank);
		printf("\n d ���������Σ�����/����/����/�ײ�/����/��ɫ/�ظ��� = %s", danmakuConfig.shieldMode);
		printf("\n e ����ģʽ�����ݱ��/�ֲ�ͼ�� = %s", danmakuConfig.debugMode);
		printf("\n\n�������� 1 ��ʾ��Ӧλ�������0 ��ʾ��Ӧλ����ر�");
		
		printf("\n------------------------\n"); 
		printf("������Ҫ�޸ĵ���Ŀǰ������\n����0��ʼת��\n");
		printf("������ ");
		char ch = getchar();
		
		fflush(stdin);
		system("cls");
		printf("DanmakuFactory v1.10\n");
		printf("%s\n\n", displayFileName);
		printf("��Ļ����\n");
		printf("------------------------\n");
		switch(ch)
		{
			case '0':
			{
				goto SETEND;
				break;
			}
			case '1':
			{
				do{
					printf("������һ������0������\n");
					printf("������ֱ��ʿ� ");
					scanf("%d", &danmakuConfig.resX);
				}while(danmakuConfig.resX <= 0);
				break;
			}
			case '2':
			{
				do{
					printf("������һ������0������\n");
					printf("������ֱ��ʸ� ");
					scanf("%d", &danmakuConfig.resY);
				}while(danmakuConfig.resY <= 0);
				break;
			}
			case '3':
			{
				do{
					printf("������һ������0������\n");
					printf("������Ļ�ٶȣ��룩 ");
					scanf("%d", &danmakuConfig.speed);
				}while(danmakuConfig.speed <= 0);
				break;
			}
			case '4': 
			{
				do{
					printf("������һ������0������\n");
					printf("�̶���Ļ�ٶȣ��룩 ");
					scanf("%d", &danmakuConfig.holdTime);
				}while(danmakuConfig.holdTime <= 0);
				break;
			}
			case '5':
			{
				do{
					printf("0 ��ʾ�����ƣ�-1 ��ʾ���ص�\n");
					printf("������һ�����ڻ����-1������\n");
					printf("��Ļ�ܶȣ����� ");
					scanf("%d", &danmakuConfig.density);
				}while(danmakuConfig.density < -1);
				break;
			}
			case '6':
			{
				do{
					printf("������һ������0������\n");
					printf("���ִ�С ");
					scanf("%d", &danmakuConfig.fontSize);
				}while(danmakuConfig.fontSize <= 0);
				break;
			}
			case '7':
			{
				printf("����������������Ŀ��ܻᵼ������\n");
				printf("���� ");
				fgets(danmakuConfig.fontName, MAX_TEXT_LENGTH, stdin);
				if(danmakuConfig.fontName[strlen(danmakuConfig.fontName + 1)] == '\n')
				{
					danmakuConfig.fontName[strlen(danmakuConfig.fontName + 1)] = '\0';
				}
				break;
			}
			case '8':
			{
				do{
					printf("������һ��0 - 255������\n");
					printf("��͸���� ");
					scanf("%d", &danmakuConfig.opacity);
				}while(danmakuConfig.opacity < 0 || danmakuConfig.opacity > 255);
				break;
			}
			case '9':
			{
				do{
					printf("������һ��0 - 4������\n");
					printf("���");
					scanf("%d", &danmakuConfig.outline);
				}while(danmakuConfig.outline < 0 || danmakuConfig.outline > 4);
				break;
			}
			case 'A':
			case 'a':
			{
				do{
					printf("������һ��0 - 4������\n");
					printf("��Ӱ ");
					scanf("%d", &danmakuConfig.shadow);
				}while(danmakuConfig.shadow < 0 || danmakuConfig.shadow > 4);
				break;
			}
			case 'B':
			case 'b':
			{
				do{
					printf("������һ��С��\n");
					printf("ʱ��ƫ�� ");
				}while(!scanf("%f", &danmakuConfig.timeShift));
				break;
			}
			case 'C':
			case 'c':
			{
				do{
					printf("������һ�����ڵ���0��������0Ϊ������\n");
					printf("�·����ף����أ� ");
					scanf("%d", &danmakuConfig.blank);
				}while(danmakuConfig.blank < 0);
				break;
			}
			case 'D':
			case 'd':
			{
				printf("˳���������е���ͬ 0 ��ʾ�رգ�1 ��ʾ����\n");
				printf("���������Σ�����/����/����/�ײ�/����/��ɫ/�ظ��� ");
				fgets(danmakuConfig.shieldMode, 8, stdin);
				break;
			}
			case 'E':
			case 'e':
			{
				printf("˳���������е���ͬ 0 ��ʾ�رգ�1 ��ʾ����\n");
				printf("����ģʽ�����ݱ��/�ֲ�ͼ�� ");
				fgets(danmakuConfig.debugMode, 3, stdin);
				break;
			}
		}
		fflush(stdin); 
	}
	SETEND:;
	
	/*д�����ļ�*/
	if((configFile = fopen("DanmakuFactoryConfig.bin", "wb")) != NULL)
	{
		fwrite(&danmakuConfig, sizeof(CONFIG), 1, configFile);
		fclose(configFile);
	}
	
	/*��ʼת��*/
	system("cls");
	printf("DanmakuFactory v%s\n", VERSION);
	printf("%s\n", displayFileName);
	printf("------------------------\n");
	int rt;
	printf("\n===== transform now start =====\n");
	DANMAKU *danmakuPoorHead = NULL;
	clock_t startTime, midTime, endTime;
	midTime = startTime = clock();
	
	
	/*�����ļ�*/ 
	rt = ReadXml(inputFileName, &danmakuPoorHead, "n", danmakuConfig.timeShift);
	endTime = clock();
	PrintFunctionDebugData("ReadXml", endTime - midTime, rt);
	
	
	/*�Ե�Ļ�ذ�ʱ�����Ͱ����*/
	midTime = clock();
	rt = SortList(&danmakuPoorHead);
	endTime = clock();
	PrintFunctionDebugData("SortList", endTime - midTime, rt);
	
	
	/*дass*/ 
	midTime = clock();
	rt = WriteAss(outputFileName,/*����ļ�*/
				  danmakuPoorHead,/*ͷָ��*/
				  danmakuConfig.resX, danmakuConfig.resY,/*�ֱ��ʿ�/�ֱ��ʸ�*/
				  danmakuConfig.fontSize, danmakuConfig.fontName,/*�ֺ�/����(utf-8)*/
				  danmakuConfig.shadow, danmakuConfig.outline,/*��Ӱ(0-4)/���(0-4)*/
				  danmakuConfig.speed, danmakuConfig.holdTime,/*�����ٶȣ�������Ļ��/ͣ��ʱ�䣨������Ļ��*/ 
				  danmakuConfig.density, danmakuConfig.opacity,/*�ܶ�/��͸����*/
				  danmakuConfig.blank, /*�²�����*/
				  danmakuConfig.shieldMode,/*���Σ�����/����/����/�ײ�/����/��ɫ/�ظ���*/
				  NULL);/*����ģʽ������ͳ��/�ֲ�ͼ��*/
	endTime = clock();
	PrintFunctionDebugData("WriteAss", endTime - midTime, rt);
	
	
	/*����ģʽ ���ﵥ������д����ģʽ���ݵĺ���ֻ��Ϊ�˷�����ԣ�����ֱ����дass���������ϲ���ģʽ����*/
	midTime = clock();
	rt = WriteAssDebugPath(outputFileName, danmakuPoorHead, danmakuConfig.debugMode,
						   danmakuConfig.speed, danmakuConfig.holdTime, danmakuConfig.shieldMode);
	endTime = clock();
	PrintFunctionDebugData("WriteAssDebugPath", endTime - midTime, rt);
	
	
	printf("\n===== end of debug data =====\n");
	printf("\n\n�������!\n����ļ�Ϊ %s.ass\n\n", displayFileName);
	printf("\nMade by TIKM\n");
	endTime = clock();
	PrintFunctionDebugData("main", endTime - startTime, 0);
	system("pause"); 
	return 0;
}

void PrintFunctionDebugData(char *name, long wasteTime, int returnValue)
{
	printf("\nFunction %s", name);
	if(wasteTime > 1000)
	{
		printf(" waste %ld %03ld ms ", wasteTime / 1000, wasteTime % 1000);
	}
	else
	{
		printf(" waste %ld ms ", wasteTime);
	}
	printf("and return value %d\n", returnValue);
	return;
}

/*
�������ַ����еĵ�б�ܱ��˫б����Ϊ·��
������
�ַ������ַ
����ֵ��
���������ַ 
*/
char *AddSlash(char *ipStr)
{
	char tempStr[1000];
	char *tempPtr = tempStr;
	char *ipPtr = ipStr;
	
	while(*ipPtr != '\0')
	{
		if(*ipPtr == '\\')
		{
			*(tempPtr + 1) = *tempPtr = '\\';
			tempPtr++;
		}
		else
		{
			*tempPtr = *ipPtr;
		}
		tempPtr++;
		ipPtr++;
	}
	*tempPtr = '\0';
	strcpy(ipStr, tempStr);
	return ipStr;
}
