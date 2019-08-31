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

#include "DanmakuFactory.h"
#include "DanmakuConvert.h"
#include "ui.c"
#include "ReadFile.c"
#include "WriteFile.c"
#include "StringProcessing.c" 
#include "ListProcessing.c"

int main(int argc, char **argv)
{
	int cnt;
	char key;/*�Ӽ��̻�ȡ�ļ�ֵ*/ 
	char firSelected, secSelected, thiSelected;/*һ�������˵���ѡ����*/ 
	
	char tempText[MAX_TEXT_LENGTH];
	
	FILE *configFilePtr;/*�����ļ�ָ��*/ 
	CONFIG defaultConfig;/*Ĭ������*/
	CONFIG *inputFile = NULL;/*�����������*/ 
	char configFilePath[MAX_TEXT_LENGTH];/*�����ļ�·��*/ 
	
	/*winAPI��дini��֧�����·�� �����·��ת��Ϊ����·��*/
	getcwd(configFilePath, MAX_TEXT_LENGTH);
	sprintf(configFilePath, "%s\\DanmakuFactoryConfig.ini", configFilePath);
	
	/*��ȡ�����ļ�*/
	readConfigFile(&defaultConfig, configFilePath);
	if (!access("DanmakuFactoryConfig.bin", 0))
	{/*�����������ļ�����*/
		printf("DanmakuFactory v%s\n", VERSION);
		printf("\n--------------------------------------------------------"
			   "\n[!] ��鵽�ɰ汾�����ļ�DanmakuFactoryConfig.bin"
			   "\n    ԭ�����������ļ�������֧��"
			   "\n    ������Y��yת��Ϊ�°汾 ԭ�����ļ����ᱻɾ��"
			   "\n--------------------------------------------------------"
			   "\n��ѡ�� ");
		firSelected = getchar();
		if((configFilePtr = fopen("DanmakuFactoryConfig.bin", "rb")) != NULL)
		{
			OLDCONFIG oldConfig;
			fread(&oldConfig, sizeof(oldConfig), 1, configFilePtr);
			defaultConfig.resX = oldConfig.resX;
			defaultConfig.resY = oldConfig.resY;
			defaultConfig.rollTime = oldConfig.rollTime;
			defaultConfig.holdTime = oldConfig.holdTime;
			defaultConfig.density = oldConfig.density;
			defaultConfig.outline = oldConfig.outline;
			defaultConfig.shadow = oldConfig.shadow;
			defaultConfig.fontSize = oldConfig.fontSize;
			defaultConfig.blank = oldConfig.blank;
			strcpy(defaultConfig.fontName, oldConfig.fontName);
			strcpy(defaultConfig.blockMode, oldConfig.blockMode);
			strcpy(defaultConfig.debugMode, oldConfig.debugMode);
			fclose(configFilePtr);
		}
		else
		{
			firSelected = 'n';
			printf("[X] ��ȡԭ�����ļ�ʧ��\n");
		}
		
		if (firSelected == 'Y' || firSelected == 'y')
		{
			if (writeConfigFile(defaultConfig, configFilePath) == FALSE)
			{
				printf("[X] д�����ļ�ʧ�� û�н����κ��ļ��޸�\n");
			}
			else
			{
				if (remove("DanmakuFactoryConfig.bin"))
				{
					printf("[!] ԭ�����ļ�ɾ��ʧ�� ���ֶ�ɾ��\n");
				}
				else
				{
					printf("[!] ת����� ԭ�����ļ���ɾ��\n");
				}
			}
			system("pause");
		}
		else
		{
			printf("   û�н����κ��ļ��޸�\n"); 
		}
	}
	
	/*ת������*/
	if (argc == 1)
	{/*ֱ������*/
		/*��ȡ�����ļ��û��趨�Ļ�������С Ӧ������ProgramSet*/
		int bufferX = GetPrivateProfileInt("ProgramSet", "bufferX", 0, configFilePath);
		int bufferY = GetPrivateProfileInt("ProgramSet", "bufferY", 0, configFilePath);
		if (bufferX == 0 || bufferY == 0)
		{/*����û�д����DanmakuSetҲͬ�����Զ�����*/
			bufferX = GetPrivateProfileInt("DanmakuSet", "bufferX", 512, configFilePath);
			bufferY = GetPrivateProfileInt("DanmakuSet", "bufferY", 2096, configFilePath);
		}
		/*�趨��������С*/
		sprintf(tempText, "mode con cols=%d lines=%d", bufferX, bufferY);
		system(tempText);
		
		/*��ȡ�����ļ��û��趨�Ĵ����С Ӧ������ProgramSet*/
		int windowX = GetPrivateProfileInt("ProgramSet", "windowX", 0, configFilePath);
		int windowY = GetPrivateProfileInt("ProgramSet", "windowY", 0, configFilePath);
		if (windowX == 0 || windowY == 0)
		{/*����û�д����DanmakuSetҲͬ�����Զ�����*/
			windowX = GetPrivateProfileInt("DanmakuSet", "windowX", GetSystemMetrics(SM_CXSCREEN)/2, configFilePath);
			windowY = GetPrivateProfileInt("DanmakuSet", "windowY", GetSystemMetrics(SM_CYSCREEN)/1.5, configFilePath);
		}
		/*�趨���ڴ�С*/
		MoveWindow(GetConsoleWindow(), 
				   (GetSystemMetrics(SM_CXSCREEN) - windowX) / 2,
				   (GetSystemMetrics(SM_CYSCREEN) - windowY) / 2,
				   windowX, windowY, TRUE);
		/*��ֹ�϶��ı䴰�ڴ�С*/
		SetWindowLongPtr(GetConsoleWindow(), GWL_STYLE, 
			  GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
		
		MAINPAGE:
		system("cls");
		showCursor(FALSE);/*����ָ��*/
		
		printf("DanmakuFactory v%s\n", VERSION);
		printf("--------------------------------"
			   "\n\n\n\n"
			   "--------------------------------"
			   "\n������ѡ��ǰ���ѡ�� ENTERȷ��"
			   );
		
		key = 0;
		firSelected = 0;
		while (key != ENTER)
		{
			printMenuText(2, 2, firSelected, "> ", 0x0F, 0xF0, FALSE, 3,
						  "0 ��ʼת��          ",
						  "1 ���������        ",
						  "2 �˳�              "
						);
			fflush(stdin);
			key = getch();
			fflush(stdin);
			if (key == UP)
			{
				if (firSelected == 0)
				{
					firSelected = 2;
				}
				else
				{
					firSelected--;
				}
			}
			else if (key == DOWN)
			{
				if (firSelected == 2)
				{
					firSelected = 0;
				}
				else
				{
					firSelected++;
				}
			}
			else if (key >= '0' && key <= '2')
			{
				firSelected = key - '0';
			}
		}
		
		showCursor(TRUE);/*��ʾָ��*/
		if (firSelected == 0)
		{/*��ת��*/
			int i = 0;
			system("cls");
			printf("DanmakuFactory v%s\n", VERSION);
			printf("--------------------------------");
			printf("\n������Ҫת�����ļ���·��\n");
			do {
				if (i % 16 == 0)
				{
					/*�������ڴ��·�����ڴ�*/
					if ((inputFile = (CONFIG *)realloc(inputFile, (i+16)*sizeof(CONFIG))) == NULL)
					{
						printf("\n[X] �ڴ�����ʧ��\n");
						system("pause");
						exit(1);
					}
				}
				
				/*�����ֵ*/
				inputFile[i] = defaultConfig;/*�����ȶ�ȫ��ֵ�ٽ�����ֱ�Ӷ���ֵ�����ݸ�ֵ*/
				//strcpy(inputFile[i].outputFormat, "ass");
				
				inputFile[i].timeShift = 0.00;
				
				/*���������ļ�·��*/
				if (i == 1)
				{
					printf("���������ļ�·��������ת��������س�����\n");
				}
				printf("%d > ", i + 1);
				fgets(inputFile[i].input, MAX_TEXT_LENGTH, stdin);
				if((inputFile[i].input)[strlen(inputFile[i].input) - 1] == '\n')
				{
					(inputFile[i].input)[strlen(inputFile[i].input) - 1] = '\0';
				}
				deQuotMarks(inputFile[i].input);
				sprintf(inputFile[i].output, "%s.%s", inputFile[i].input, "ass");/*��������ļ�·��*/ 
				
				i++;
			} while ((inputFile[i - 1].input)[0] != '\0');
			i--;/*���һ��û���������Զ���*/
			
			/*û���κ����ݾ�ֱ�ӷ���*/ 
			if (i == 0)
			{
				free(inputFile);
				inputFile = NULL;/*�������´�realloc�ͻ����*/
				goto MAINPAGE;
			}
			
			/*�趨ҳ*/
			int nowFile = 0;
			int page = 0;
			secSelected = 6;
			
			/*ֻ��һ�����ݾ�û�зֿ�ȫ�����һ����������*/
			if (i == 1)
			{
				nowFile = 1;
			}
			
			while (TRUE)
			{
				char displayText[4][MAX_TEXT_LENGTH];/*ǰ8�и�Ŀ¼��ʾ ����ĸ��˵�*/
				
				key = 0;
				system("cls");
				showCursor(FALSE);/*����ָ��*/
				
				/*��ӡ����*/ 
				printf("DanmakuFactory v%s\n", VERSION);
				printf("\n�����յ� %d ���ļ�", i);
				printf("\n--------------------------------");
				printf("\n\n\n\n\n\n\n\n");
				printf("\n����һҳ   ����һҳ"
					   "\n--------------------------------"
					   );
				printf("\n"
					   "\n"
					   "\n"
					   "\n"
					   "\n"
					   "\n"
					   "\n" 
					   "\n--------------------------------"
					   "\n����ѡ��  ENTER����"
					   );
				
				/*��ӡ��ǰҳ����ʾ���ļ��б�*/ 
				for (cnt = 0; cnt < 8 && page * 8 + cnt < i; cnt++)
				{
					setPos(0, 4 + cnt);
					printf("%d %s", page*8 + cnt + 1, inputFile[page*8 + cnt].input);
				}
				setPos(21, 12);
				printf("��%dҳ ��%dҳ", page + 1, (int)(ceil(i / 8.0))); 
				
				/*�˵�����Ҫ��ʾ���ı� ��䵽����displayText*/
				if (nowFile == 0)
				{
					sprintf(displayText[0], "1 ��ǰ���� ȫ���ļ�");
					sprintf(displayText[1], "2 ת��Ϊ --");
					sprintf(displayText[2], "3 ʱ��ƫ�� %.2f", inputFile[0].timeShift);
					for (cnt = 1; cnt < i; cnt++)
					{
						if (fabs(inputFile[cnt].timeShift - inputFile[cnt - 1].timeShift) > FLOAT_EPS)
						{
							sprintf(displayText[2], "3 ʱ��ƫ�� --");
							break;
						}
					}
					
					if (inputFile[0].outputEncoding == ANSI)
					{
						sprintf(displayText[3], "4 ������� ANSI");
					}
					else
					{
						sprintf(displayText[3], "4 ������� UTF-8");
					}
					
					for (cnt = 1; cnt < i; cnt++)
					{
						if (inputFile[cnt].outputEncoding != inputFile[cnt - 1].outputEncoding)
						{
							sprintf(displayText[3], "4 ������� --");
							break;
						}
					}
				}
				else
				{
					sprintf(displayText[0], "1 ��ǰ���� %d", nowFile);
					sprintf(displayText[1], "2 ת��Ϊ %s", inputFile[nowFile - 1].output);
					sprintf(displayText[2], "3 ʱ��ƫ�� %.2f", inputFile[nowFile - 1].timeShift);
					if (inputFile[nowFile - 1].outputEncoding == ANSI)
					{
						sprintf(displayText[3], "4 ������� ANSI");
					}
					else
					{
						sprintf(displayText[3], "4 ������� UTF-8");
					}
				}
				
				
				/*��ȡ��������*/ 
				while (key != ENTER)
				{
					if (key == UP)
					{
						if (secSelected == 0)
						{
							secSelected = 6;
						}
						else
						{
							secSelected--;
						}
					}
					else if (key == DOWN)
					{
						if (secSelected == 6)
						{
							secSelected = 0;
						}
						else
						{
							secSelected++;
						}
					}
					else if (key == LEFT || key == RIGHT)
					{
						if (key == LEFT)
						{
							if (page == 0)
							{
								page = (int)(ceil(i / 8.0)) - 1;/*����ȡ��*/
							}
							else
							{
								page--;
							}
						}
						else
						{
							if (page == (int)(ceil(i / 8.0)) - 1)
							{
								page = 0;/*����ȡ��*/
							}
							else
							{
								page++;
							}
						}
						break;/*����ѭ��Ȼ��ˢ������ҳ��*/	
					}
					else if (key == '0')
					{
						secSelected = 6;
					}
					else if (key >= '1' && key <= '6')
					{
						secSelected = key - '0' - 1;
					}
					
					/*��ӡ�˵�*/
					printMenuText(0, 14, secSelected, "> ", 0x0F, 0xF0, FALSE, 7,
								  displayText[0],
								  displayText[1],
								  displayText[2],
								  displayText[3],
								  "5 ���൯Ļ����",
								  "6 ���浱ǰ����", 
								  "0 ��ʼȫ��ת��"
								 );
					fflush(stdin);
					key = getch();
					fflush(stdin);
				}/*�����˵�ѡ��ѭ��*/ 
				
				if (key == LEFT || key == RIGHT)
				{/*�����ҳ��ֱ��ˢ������ҳ��*/
					continue;
				}
				
				showCursor(TRUE);/*��ʾָ��*/
				/*�����û�ѡ��Ľ�һ������*/
				if (secSelected == 0)
				{/*���õ�ǰҪ���õ��ļ�*/
					setPos(0, 22);
					if (i == 1)
					{
						printf("[!] ��ǰ���޷����ã���Ϊֻ��һ���ļ�\n");
						system("pause"); 
					}
					else
					{
						printf("��������Ҫ���õ��ļ�ǰ��� 0��ʾȫ��\n");
						do {
							fflush(stdin);
							printf("������(0-%d) ", i);
							fgets(tempText, MAX_TEXT_LENGTH, stdin);
							if(tempText[strlen(tempText) - 1] == '\n')
							{
								tempText[strlen(tempText) - 1] = '\0';
							}
							nowFile = strToInt(tempText);/*�Ȼ�ȡ�ַ����ж��Ƿ��зǷ��ַ�*/
						} while ((nowFile < 0 || nowFile > i) && !isDesignatedChar(tempText, "0123456789"));
					}
				}
				else if (secSelected == 1)
				{/*�����ļ�·��*/ 
					setPos(0, 22);
					if (nowFile == 0)
					{/*ȫ�����ûᵼ�´���*/
						printf("[X] ����ͬʱ�趨ȫ���ļ������·��\n");
						system("pause"); 
					}
					else
					{/*�ֲ�����*/ 
						printf("������ %d ���ļ������·��\n", nowFile);
						printf("������(����׺��) ");
						fgets(inputFile[nowFile - 1].output, MAX_TEXT_LENGTH, stdin);
						if((inputFile[nowFile - 1].output)[strlen(inputFile[nowFile - 1].output) - 1] == '\n')
						{
							(inputFile[nowFile - 1].output)[strlen(inputFile[nowFile - 1].output) - 1] = '\0';
						}
						deQuotMarks(inputFile[nowFile - 1].output);
					}
				}
				else if (secSelected == 2)
				{/*����ʱ��ƫ����*/ 
					setPos(0, 22);
					if (nowFile == 0)
					{/*ȫ������*/ 
						printf("������ȫ���ļ���ʱ��ƫ��\n", nowFile);
						do {
							printf("������(С��) ");
							fgets(tempText, MAX_TEXT_LENGTH, stdin);
							if(tempText[strlen(tempText) - 1] == '\n')
							{
								tempText[strlen(tempText) - 1] = '\0';
							}
						} while (!isDesignatedChar(tempText, "-.0123456789"));/*�ж��Ƿ��зǷ��ַ�*/
						float tempTimeShift = strToFloat(tempText);
						for (cnt = 0; cnt < i; cnt++)
						{
							inputFile[cnt].timeShift = tempTimeShift;
						}
						
					}
					else
					{/*�ֲ�����*/
						printf("������ %d ���ļ���ʱ��ƫ��\n", nowFile);
						do {
							printf("������(С��) ");
							fgets(tempText, MAX_TEXT_LENGTH, stdin);
							if(tempText[strlen(tempText) - 1] == '\n')
							{
								tempText[strlen(tempText) - 1] = '\0';
							}
						} while (!isDesignatedChar(tempText, "-.0123456789"));/*�ж��Ƿ��зǷ��ַ�*/ 
						inputFile[nowFile - 1].timeShift = strToFloat(tempText);
					}
					
				}
				else if (secSelected == 3)
				{/*�����������*/ 
					setPos(0, 22);
					showCursor(FALSE);/*����ָ��*/
					if (nowFile == 0)
					{/*����ȫ��*/ 
						printf("��ѡ��ȫ���ļ����������\n", nowFile);
					}
					else
					{/*���õ���*/ 
						printf("��ѡ�� %d ���ļ����������\n", nowFile);
					}
					
					/*ͨ��ѡ��˵�ѡ��һ����*/
					key = 0;
					thiSelected = 0; 
					while (key != ENTER)
					{
						printMenuText(0, 23, thiSelected, "> ", 0x0F, 0xF0, FALSE, 2,
									  "0 ANSI           ",
									  "1 UTF-8          ");
						printf("\n\n[!] Ŀǰ�ı���ת��������ȫ�ɿ�");
						printf("\n    ��֪GB2312תUTF-8�м��ʷ�������");
						printf("\n    ��ر���ת���������ı��༭�����");
						fflush(stdin);
						key = getch();
						fflush(stdin);
						if (key == UP)
						{
							if (thiSelected == 0)
							{
								thiSelected = 1;
							}
							else
							{
								thiSelected--;
							}
						}
						else if (key == DOWN)
						{
							if (thiSelected == 1)
							{
								thiSelected = 0;
							}
							else
							{
								thiSelected++;
							}
						}
						else if (key >= '0' && key <= '1')
						{
							thiSelected = key - '0';
						}
					}/*��������ѡ��˵�*/ 
					 
					/*��Ӧ�û���ѡ��*/ 
					if (thiSelected == 0)
					{
						if (nowFile == 0)
						{/*ȫ���޸�*/ 
							for (cnt = 0; cnt < i; cnt++)
							{
								inputFile[cnt].outputEncoding = ANSI;
							}
						}
						else
						{/*�ֲ��޸�*/
							inputFile[nowFile - 1].outputEncoding = ANSI;
						}
						
					}
					else
					{
						if (nowFile == 0)
						{/*ȫ���޸�*/ 
							for (cnt = 0; cnt < i; cnt++)
							{
								inputFile[cnt].outputEncoding = UTF_8;
							}
						}
						else
						{/*�ֲ��޸�*/ 
							inputFile[nowFile - 1].outputEncoding = UTF_8;
						}
					}
				}
				else if (secSelected == 4)
				{/*������Ļ����*/ 
					char setDisplay[20][MAX_TEXT_LENGTH];
					CONFIG newSet;
					
					/*
					��ȡָ���ļ��ĵ�Ļ����
					�����±��0��ʼ��nowFile��1��ʼ����˶�ĳһ���ļ�Ҫ�� 1
					����0��ʾȫ�� ���nowFile Ϊ 0 ʱĬ��ȡ��һ���ļ������ 1
					*/
					int dSetSelected = 0;
					int dSetKey; 
					int shift = 1;
					if (nowFile == 0)
					{
						shift = 0;
					}
					
					/*�趨ҳ��UI*/
					do {
						system("cls");
						dSetKey = 0;
						showCursor(FALSE);/*����ָ��*/
						printf("> DanmakuFactory ��Ļ����", nowFile);
						sprintf(setDisplay[0], "   1 �ֱ��ʿ� %dpx", inputFile[nowFile - shift].resX);
						sprintf(setDisplay[1], "   2 �ֱ��ʸ� %dpx", inputFile[nowFile - shift].resY);
						sprintf(setDisplay[2], "   3 ������Ļʱ�� %d��", inputFile[nowFile - shift].rollTime);
						sprintf(setDisplay[3], "   4 �̶���Ļʱ�� %d��", inputFile[nowFile - shift].holdTime);
						if (inputFile[nowFile - shift].density == -1)
						{
							sprintf(setDisplay[4], "   5 ��Ļ�ܶ� ���ص�(-1)");
						}
						else if (inputFile[nowFile - shift].density == 0)
						{
							sprintf(setDisplay[4], "   5 ��Ļ�ܶ� ������(0)");
						}
						else
						{
							sprintf(setDisplay[4], "   5 ��Ļ�ܶ� %d��", inputFile[nowFile-shift].density);
						}
						sprintf(setDisplay[5], "   6 ���ִ�С %d", inputFile[nowFile - shift].fontSize);
						if (isUtf8(defaultConfig.fontName) == TRUE)
						{/*������˵���ı�*/
							transcoding(UTF_8, ANSI, inputFile[nowFile - shift].fontName, NULL, 0);
						}
						sprintf(setDisplay[6], "   7 ���� %s", inputFile[nowFile - shift].fontName);
						sprintf(setDisplay[7], "   8 ��� %d", inputFile[nowFile - shift].outline);
						sprintf(setDisplay[8], "   9 ��Ӱ %d", inputFile[nowFile - shift].shadow);
						sprintf(setDisplay[9], "   a ��͸���� %d", inputFile[nowFile - shift].opacity);
						sprintf(setDisplay[10], "   b �²����� %dpx", inputFile[nowFile - shift].blank);
						if ((inputFile[nowFile - shift].blockMode)[0] == '0')
						{
							sprintf(setDisplay[11], "   c �������[������]");
						}
						else
						{
							sprintf(setDisplay[11], "   c �������[����]");
						}
						
						if ((inputFile[nowFile - shift].blockMode)[1] == '0')
						{
							sprintf(setDisplay[12], "   d ���ҹ���[������]");
						}
						else
						{
							sprintf(setDisplay[12], "   d ���ҹ���[����]");
						}
						
						if ((inputFile[nowFile - shift].blockMode)[2] == '0')
						{
							sprintf(setDisplay[13], "   e �����̶�[������]");
						}
						else
						{
							sprintf(setDisplay[13], "   e �����̶�[����]");
						}
						
						if ((inputFile[nowFile - shift].blockMode)[3] == '0')
						{
							sprintf(setDisplay[14], "   f �ײ��̶�[������]");
						}
						else
						{
							sprintf(setDisplay[14], "   f �ײ��̶�[����]");
						}
						
						if ((inputFile[nowFile - shift].blockMode)[4] == '0')
						{
							sprintf(setDisplay[15], "   g ����    [������]");
						}
						else
						{
							sprintf(setDisplay[15], "   g ����    [����]");
						}
						
						if ((inputFile[nowFile - shift].blockMode)[5] == '0')
						{
							sprintf(setDisplay[16], "   h ��ɫ    [������]");
						}
						else
						{
							sprintf(setDisplay[16], "   h ��ɫ    [����]");
						}
						
						if ((inputFile[nowFile - shift].blockMode)[6] == '0')
						{
							sprintf(setDisplay[17], "   i �����ظ�[������]");
						}
						else
						{
							sprintf(setDisplay[17], "   i �����ظ�[����]");
						}
						
						if ((inputFile[nowFile - shift].debugMode)[0] == '0')
						{
							sprintf(setDisplay[18], "   j ���ݱ��[�ر�]");
						}
						else
						{
							sprintf(setDisplay[18], "   j ���ݱ��[����]");
						}
						
						if ((inputFile[nowFile - shift].debugMode)[1] == '0')
						{
							sprintf(setDisplay[19], "   k ͳ��ͼ  [�ر�]");
						}
						else
						{
							sprintf(setDisplay[19], "   k ͳ��ͼ  [����]");
						}
						
						if (nowFile == 0)
						{/*����ȫ��*/
							printf("(ȫ���ļ�)\n");
							/*��������ļ������Ƿ���ͬ ��ͬ����--*/ 
							for (cnt = 1; cnt < i; cnt++)
							{
								if (inputFile[cnt].resX != inputFile[cnt - 1].resX)
								{
									sprintf(setDisplay[0], "   1 �ֱ��ʿ� --");
								}
								if (inputFile[cnt].resY != inputFile[cnt - 1].resY)
								{
									sprintf(setDisplay[1], "   2 �ֱ��ʿ� --");
								}
								if (inputFile[cnt].rollTime != inputFile[cnt - 1].rollTime)
								{
									sprintf(setDisplay[2], "   3 ������Ļʱ�� --");
								}
								if (inputFile[cnt].holdTime != inputFile[cnt - 1].holdTime)
								{
									sprintf(setDisplay[3], "   4 �̶���Ļʱ�� --");
								}
								if (inputFile[cnt].density != inputFile[cnt - 1].density)
								{
									sprintf(setDisplay[4], "   5 ��Ļ�ܶ� --");
								}
								if (inputFile[cnt].fontSize != inputFile[cnt - 1].fontSize)
								{
									sprintf(setDisplay[5], "   6 ���ִ�С --");
								}
								if (strcmp(inputFile[cnt].fontName, inputFile[cnt - 1].fontName))
								{
									sprintf(setDisplay[6], "   7 ���� --");
								}
								if (inputFile[cnt].outline != inputFile[cnt - 1].outline)
								{
									sprintf(setDisplay[7], "   8 ��� --");
								}
								if (inputFile[cnt].shadow != inputFile[cnt - 1].shadow)
								{
									sprintf(setDisplay[8], "   9 ��Ӱ --");
								}
								if (inputFile[cnt].opacity != inputFile[cnt - 1].opacity)
								{
									sprintf(setDisplay[9], "   a ��͸���� --");
								}
								if (inputFile[cnt].blank != inputFile[cnt - 1].blank)
								{
									sprintf(setDisplay[10], "   b �²����� --");
								}
								if ((inputFile[cnt].blockMode)[0] != 
									(inputFile[cnt - 1].blockMode)[0])
								{
									sprintf(setDisplay[11], "   c ������� --");
								}
								if ((inputFile[cnt].blockMode)[1] != 
									(inputFile[cnt - 1].blockMode)[1])
								{
									sprintf(setDisplay[12], "   d ���ҹ��� --");
								}
								if ((inputFile[cnt].blockMode)[2] != 
									(inputFile[cnt - 1].blockMode)[2])
								{
									sprintf(setDisplay[13], "   e �����̶� --");
								}
								if ((inputFile[cnt].blockMode)[3] != 
									(inputFile[cnt - 1].blockMode)[3])
								{
									sprintf(setDisplay[14], "   f �ײ��̶� --");
								}
								if ((inputFile[cnt].blockMode)[4] != 
									(inputFile[cnt - 1].blockMode)[4])
								{
									sprintf(setDisplay[15], "   g ����     --");
								}
								if ((inputFile[cnt].blockMode)[5] != 
									(inputFile[cnt - 1].blockMode)[5])
								{
									sprintf(setDisplay[16], "   h ��ɫ     --");
								}
								if ((inputFile[cnt].blockMode)[6] != 
									(inputFile[cnt - 1].blockMode)[6])
								{
									sprintf(setDisplay[17], "   i �����ظ� --");
								}
								if ((inputFile[cnt].debugMode)[0] != 
									(inputFile[cnt - 1].debugMode)[0])
								{
									sprintf(setDisplay[18], "   j ���ݱ�� --");
								}
								if ((inputFile[cnt].debugMode)[1] != 
									(inputFile[cnt - 1].debugMode)[1])
								{
									sprintf(setDisplay[19], "   k ͳ��ͼ   --");
								}
							}
						}
						else
						{/*���õ���*/ 
							printf("(%d���ļ�)\n", nowFile);
						}
						printf("--------------------------------\n");
						
						while (dSetKey != ENTER)
						{
							if (dSetKey == UP)
							{
								if (dSetSelected == 0)
								{
									dSetSelected = 23;
								}
								else
								{
									dSetSelected--;
								}
							}
							else if (dSetKey == DOWN)
							{
								if (dSetSelected == 23)
								{
									dSetSelected = 0;
								}
								else
								{
									dSetSelected++;
								}
							}
							else if (dSetKey >= '1' && dSetKey <= '9')
							{
								dSetSelected = dSetKey - '0';
							}
							else if (dSetKey >= 'a' && dSetKey <= 'b')
							{
								dSetSelected = dSetKey - 'a' + 10;
							}
							else if (dSetKey >= 'c' && dSetKey <= 'i')
							{
								dSetSelected = dSetKey - 'c' + 13;
							}
							else if (dSetKey >= 'j' && dSetKey <= 'k')
							{
								dSetSelected = dSetKey - 'j' + 21;
							}
							else if (dSetKey == '0')
							{
								dSetSelected = 23;
							}
										
							printMenuText(0, 2, dSetSelected, "> ", 0x0F, 0xF0, FALSE, 24,
										  "�����趨",
										  setDisplay[0], setDisplay[1], setDisplay[2],
										  setDisplay[3], setDisplay[4], setDisplay[5],
										  setDisplay[6], setDisplay[7], setDisplay[8],
										  setDisplay[9], setDisplay[10],
										  "����������", setDisplay[11], setDisplay[12],
										  setDisplay[13], setDisplay[14], setDisplay[15],
										  setDisplay[16], setDisplay[17],
										  "����ģʽ",
										  setDisplay[18], setDisplay[19],
										  "0 ��ɲ��˳�");
							printf("\n--------------------------------");
							printf("\n������ѡ��ǰ���ѡ�� ENTER�޸�");
							fflush(stdin);
							dSetKey = getch();
							fflush(stdin);
						}
						
						/*ִ��ѡ��*/
						setPos(0, 27);
						
						showCursor(TRUE);/*��ʾָ��*/
						if (dSetSelected == 1)
						{
							printf("�趨�µķֱ��ʿ�(px)            \n");						
							inputFile[nowFile - shift].resX = inputInt(1, INT_MAX, "������(>0) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].resX = inputFile[0].resX;
								}
							}
						}
						else if (dSetSelected == 2) 
						{
							printf("�趨�µķֱ��ʸ�(px)            \n");
							inputFile[nowFile - shift].resY = inputInt(1, INT_MAX, "������(>0) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].resY = inputFile[0].resY;
								}
							}
						}
						else if (dSetSelected == 3) 
						{
							printf("�趨�µĹ�����Ļ�����ٶ�(��)    \n");
							inputFile[nowFile - shift].rollTime = inputInt(1, INT_MAX, "������(>0) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].rollTime = inputFile[0].rollTime;
								}
							}
						}
						else if (dSetSelected == 4) 
						{
							printf("�趨�µĹ̶���Ļͣ��ʱ��(��)    \n");
							inputFile[nowFile - shift].holdTime = inputInt(1, INT_MAX, "������(>0) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].holdTime = inputFile[0].holdTime;
								}
							}
						}
						else if (dSetSelected == 5) 
						{
							printf("�趨�µĵ�Ļ�ܶ�(��)(0 ��ʾ������ -1 ��ʾ���ص�)\n");
							inputFile[nowFile - shift].density = inputInt(-1, INT_MAX, "������(>=-1) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].density = inputFile[0].density;
								}
							}
						}
						else if (dSetSelected == 6) 
						{
							printf("�趨�µ����ִ�С                \n");
							inputFile[nowFile - shift].fontSize = inputInt(1, INT_MAX, "������(>0) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].fontSize = inputFile[0].fontSize;
								}
							}
						}
						else if (dSetSelected == 7) 
						{
							printf("�趨�µ�����                    \n������ ");
							fgets(inputFile[nowFile - shift].fontName, MAX_TEXT_LENGTH, stdin);
							if(tempText[strlen(inputFile[nowFile - shift].fontName) - 1] == '\n')
							{
								tempText[strlen(inputFile[nowFile - shift].fontName) - 1] = '\0';
							}
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									strcpy(inputFile[cnt].fontName, inputFile[0].fontName);
								}
							}
						}
						else if (dSetSelected == 8) 
						{
							printf("�趨�µ���ߴ�ϸ(0 �ر�)        \n");
							inputFile[nowFile - shift].outline = inputInt(0, 4, "������(0-4) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].outline = inputFile[0].outline;
								}
							}
						}
						else if (dSetSelected == 9) 
						{
							printf("�趨�µ���Ӱ��С(0 �ر�)        \n");
							inputFile[nowFile - shift].shadow = inputInt(0, 4, "������(0-4) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].shadow = inputFile[0].shadow;
								}
							}
						}
						else if (dSetSelected == 10) 
						{
							printf("�趨�µĲ�͸����(0 ��ȫ͸�� 255 ��͸��)\n");
							inputFile[nowFile - shift].opacity = inputInt(0, 255, "������(0-255) ");
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].opacity = inputFile[0].opacity;
								}
							}
						}
						else if (dSetSelected == 11) 
						{
							printf("�趨�µ��²����������С��������Ļ��\n");
							inputFile[nowFile - shift].blank = inputInt(0,
													inputFile[nowFile - shift].resY,
													 "������(0-%d) ", inputFile[nowFile - shift].resY);
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blank = inputFile[0].blank;
								}
							}
						}
						else if (dSetSelected == 13) 
						{
							if (inputFile[nowFile - shift].blockMode[0] == '0')
							{
								inputFile[nowFile - shift].blockMode[0] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[0] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[0] = inputFile[0].blockMode[0];
								}
							}
						}
						else if (dSetSelected == 14) 
						{
							if (inputFile[nowFile - shift].blockMode[1] == '0')
							{
								inputFile[nowFile - shift].blockMode[1] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[1] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[1] = inputFile[0].blockMode[1];
								}
							}
						}
						else if (dSetSelected == 15) 
						{
							if (inputFile[nowFile - shift].blockMode[2] == '0')
							{
								inputFile[nowFile - shift].blockMode[2] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[2] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[2] = inputFile[0].blockMode[2];
								}
							}
						}
						else if (dSetSelected == 16) 
						{
							if (inputFile[nowFile - shift].blockMode[3] == '0')
							{
								inputFile[nowFile - shift].blockMode[3] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[3] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[3] = inputFile[0].blockMode[3];
								}
							}
						}
						else if (dSetSelected == 17) 
						{
							if (inputFile[nowFile - shift].blockMode[4] == '0')
							{
								inputFile[nowFile - shift].blockMode[4] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[4] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[4] = inputFile[0].blockMode[4];
								}
							}
						}
						else if (dSetSelected == 18) 
						{
							if (inputFile[nowFile - shift].blockMode[5] == '0')
							{
								inputFile[nowFile - shift].blockMode[5] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[5] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[5] = inputFile[0].blockMode[5];
								}
							}
						}
						else if (dSetSelected == 19) 
						{
							if (inputFile[nowFile - shift].blockMode[6] == '0')
							{
								inputFile[nowFile - shift].blockMode[6] = '1';
							}
							else
							{
								inputFile[nowFile - shift].blockMode[6] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].blockMode[6] = inputFile[0].blockMode[6];
								}
							}
						}
						else if (dSetSelected == 21) 
						{
							if (inputFile[nowFile - shift].debugMode[0] == '0')
							{
								inputFile[nowFile - shift].debugMode[0] = '1';
							}
							else
							{
								inputFile[nowFile - shift].debugMode[0] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].debugMode[0] = inputFile[0].debugMode[0];
								}
							}
						}
						else if (dSetSelected == 22) 
						{
							if (inputFile[nowFile - shift].debugMode[1] == '0')
							{
								inputFile[nowFile - shift].debugMode[1] = '1';
							}
							else
							{
								inputFile[nowFile - shift].debugMode[1] = '0';
							}
							
							if (nowFile == 0)
							{/*ȫ��������Ҫ����һ���ļ�������ȫ���ļ�*/
								for (cnt = 1; cnt < i; cnt++)
								{
									inputFile[cnt].debugMode[1] = inputFile[0].debugMode[1];
								}
							}
						}
					} while (dSetSelected != 23);
				}
				else if (secSelected == 5)
				{/*��������*/
					setPos(0, 22);
					if (nowFile == 0)
					{/*ѡ��ȫ���򱣴��һ���ļ�������*/
						if (writeConfigFile(inputFile[0], configFilePath))
						{
							printf("[!] ����ɹ�                    \n");
						}
						else
						{
							printf("[X] ����ʧ��                    \n");
						}
					}
					else
					{
						if (writeConfigFile(inputFile[nowFile - 1], configFilePath))
						{
							printf("[!] ����ɹ�                    \n");
						}
						else
						{
							printf("[X] ����ʧ��                    \n");
						}
					}
					system("pause");
				}
				else if (secSelected == 6)
				{/*��ʼת��*/
					break;
				}
			}/*�����趨ҳ*/ 
			
			/*ת��*/
			int failNum = 0, existNum = 0;
			system("cls");
			printf("DanmakuFactory v%s\n", VERSION);
			printf("--------------------------------");
			printf("\n> ���ڼ������ļ��Ƿ����");
			for (cnt = 0; cnt < i; cnt++)
			{
				if (access(inputFile[cnt].output, 0) == FALSE)
				{
					existNum++;
					printf("\n%d %s", existNum, inputFile[cnt].output);
				}
			}
			if (existNum > 0)
			{
				printf("\n�������� %d ���Ѿ����ڵ��ļ����������ᱻ����"
					   "\n���б����������ø������Ļ��������С"
					   "\n����Y��y���� ���������ַ��˳�\n������ ", existNum);
				fflush(stdin);
				char ch = getchar();
				fflush(stdin);
				if (ch != 'Y' && ch != 'y')
				{
					free(inputFile);
					inputFile = NULL;
					goto MAINPAGE;
				}
			}
			
			printf("\n> �������ڿ�ʼ �� %d ���ļ�", i);
			for (cnt = 0; cnt < i; cnt++)
			{
				char code[4] = "000";
				DANMAKU *danmakuPoorHead = NULL;
				clock_t startTime, endTime;
				
				printf("\n> %d/%d ��ʼת�� ", cnt + 1, i);
				startTime = clock();
				
				/*��ȡ*/
				code[0] += readXml(inputFile[cnt].input, &danmakuPoorHead, "n", inputFile[cnt].timeShift);
				/*����ת��*/
				if (inputFile[cnt].outputEncoding == UTF_8 && isUtf8(inputFile[cnt].fontName) == FALSE)
				{/*������˵���ı�*/
					transcoding(ANSI, UTF_8, inputFile[cnt].fontName, NULL, 0);
				}
				else if (inputFile[cnt].outputEncoding == ANSI && isUtf8(inputFile[cnt].fontName) == TRUE)
				{
					transcoding(UTF_8, ANSI, inputFile[cnt].fontName, NULL, 0);
				}
				transListCoding(danmakuPoorHead, inputFile[cnt].outputEncoding);/*��ȫ����Ļ*/ 
				/*����*/
				code[1] += sortList(&danmakuPoorHead);
				/*д��*/
				code[2] += writeAss(inputFile[cnt].output,/*����ļ�*/
								    danmakuPoorHead,/*ͷָ��*/
								    inputFile[cnt].resX,/*�ֱ��ʿ�*/
								    inputFile[cnt].resY,/*�ֱ��ʸ�*/
								    inputFile[cnt].fontSize,/*�ֺ�*/
								    inputFile[cnt].fontName,/*����(utf-8)*/
								    inputFile[cnt].shadow,/*��Ӱ(0-4)*/
								    inputFile[cnt].outline,/*���(0-4)*/
								    inputFile[cnt].rollTime,/*�����ٶȣ�������Ļ��*/
								    inputFile[cnt].holdTime,/*ͣ��ʱ�䣨������Ļ��*/ 
								    inputFile[cnt].density,/*�ܶ�*/
								    inputFile[cnt].opacity,/*��͸����*/
								    inputFile[cnt].blank, /*�²�����*/
								    inputFile[cnt].blockMode,/*����*/
								    inputFile[cnt].debugMode);/*����ģʽ*/
				endTime = clock();
				
				freeList(danmakuPoorHead);
				if (strcmp(code ,"000") == 0 || strcmp(code ,"009") == 0)
				{
					printf(" -> ת���ɹ�");
					printf(" ��ʱ %d ms ״̬���� C%s", endTime - startTime, code);
					printf("\n  ����ļ� %s", inputFile[cnt].output);
				}
				else
				{
					failNum++;
					printf(" -> ת��ʧ�� ");
					printf(" ��ʱ %d ms ״̬���� C%s ", endTime - startTime, code);
					printErrInfo(code);/*���ͱ���*/
				}
				endTime = clock();
			}
			
			free(inputFile);
			inputFile = NULL;
			printf("\n--------------------------------");
			printf("\nת������ %d ���ɹ���%d��ʧ��", i - failNum, failNum);
			printf("\nMade by TIKM");
			printf("\nhttps://github.com/HITIKM/DanmakuFactory\n");
			system("pause");
		}
		else if (firSelected == 1)
		{/*���������*/
			showCursor(FALSE);/*����ָ��*/
			system("cls");
			secSelected = 1;
			key = 0;
			do {
				setPos(0, 0);
				printf("DanmakuFactory v%s\n", VERSION);
				printf("--------------------------------");
				printMenuText(0, 2, secSelected, "> ", 0x0F, 0xF0, FALSE, 10,
							  "> ��ȡ����", 
							  "  github https://github.com/HITIKM/DanmakuFactory/tree/master/release",
			                  "  �ٶ����� https://pan.baidu.com/s/1mkMaiq8AaUFUAGmv0s74vw ��ȡ�룺5vqj",
							  "> �̳�",
							  "  ���� http://tikm.org/df/help/",
							  "> ����",
							  "  ���� hkm@tikm.org",
							  "  github https://github.com/HITIKM/DanmakuFactory/issues",
							  "  ���� https://df.tikm.org/fk/ (��������˵Ļ�)",
							  "> ����");
				printf("\n--------------------------------");
				printf("\n����ѡ��  ENTER ��ת����Ӧ��ҳ");
				fflush(stdin);
				key = getch();
				fflush(stdin);
				
				if (key == UP)
				{
					if (secSelected == 0)
					{
						secSelected = 9;
					}
					else
					{
						secSelected--;
					}
				}
				else if (key == DOWN)
				{
					if (secSelected == 9)
					{
						secSelected = 0;
					}
					else
					{
						secSelected++;
					}
				}
				else if (key == ENTER && secSelected != 9)
				{
					switch (secSelected)
					{
						case 1:
						{
							system("start https://github.com/HITIKM/DanmakuFactory/tree/master/release");
							break;
						}
						case 2:
						{
							system("start https://pan.baidu.com/s/1mkMaiq8AaUFUAGmv0s74vw");
							break;
						}
						case 4:
						{
							system("start http://tikm.org/df/help/");
							break;
						}
						case 6:
						{
							system("start mailto:hkm@tikm.org");
							break;
						}
						case 7:
						{
							system("start https://github.com/HITIKM/DanmakuFactory/issues");
							break;
						}
					}
				}
				else if (key == ENTER && secSelected == 9)
				{
					break;
				}
			} while (TRUE);
			
			showCursor(TRUE);/*��ʾָ��*/ 
		}
		else if (firSelected == 2)
		{/*�˳�*/ 
			exit(0); 
		}
		goto MAINPAGE;
	}
	else
	{
		/*�����е���
		./danmakuFactory [-h][-s][-ip �����ļ���][-if �����ļ���ʽ][-op ����ļ���]
					[-of ����ļ���ʽ][-t ʱ��ƫ��][-rx �ֱ��ʿ�][-ry �ֱ��ʸ�][-rt ������Ļ����ʱ��]
					[-ht �̶���Ļ���ʱ��][-d ��Ļ�ܶ�][-fs ���ִ�С][-fn ����][-o ��͸����][-l ���]
					[-s ��Ӱ][-b �ײ�����][-bm ����ģʽ][-dm ����ģʽ]*/
		BOOL save = FALSE;
		float timeShift = 0.00;
		char inputFormat[MAX_TEXT_LENGTH] = {0};
		char outputFormat[MAX_TEXT_LENGTH] = {0};
		char inputFileName[MAX_TEXT_LENGTH] = {0}, outputFileName[MAX_TEXT_LENGTH] = {0};
		printf("DanmakuFactory v%s\n", VERSION);
		for (cnt = 1; cnt < argc; cnt++)
		{
			if (argv[cnt][0] == '-')
			{/*��ѡ��ȫ��ת��ΪСд��֧�ִ�Сд����*/
				toLower(NULL, argv[cnt]);
			}
			else
			{/*��ѡ��ֱ������*/
				continue;
			}
			
			/*������ѡ��ѡ��*/
			if ((strcmp(argv[cnt], "-h") == 0 || strcmp(argv[cnt], "--help") == 0) && argc == 2)
			{/*����*/
				printf("\n> �����е���_����");
				printf("\n  ./danmakuFactory [-h][-s][-ip �����ļ���][-if �����ļ���ʽ][-op ����ļ���]"
							"[-of ����ļ���ʽ][-oe ����ļ�����][-t ʱ��ƫ��][-rx �ֱ��ʿ�][-ry �ֱ��ʸ�]"
							"[-rt ������Ļ����ʱ��][-ht �̶���Ļ���ʱ��][-d ��Ļ�ܶ�]"
							"[-fs ���ִ�С][-fn ����][-o ��͸����][-l ���][-sd ��Ӱ]"
							"[-b �ײ�����][-bm ����ģʽ][-dm ����ģʽ]"

					   "\n\n> ѡ�����"
					   "\n  -h, --help ��ʾ�����ı������Ǵ�������ѡ��"
					   "\n  -s, --set �����ñ��浽�ļ���û������ѡ�����г���ǰ�ļ��е�����ֵ��"
							   "ת��ʱ�����ѡ���ʾ���������ñ��浽�ļ��У���������ļ���-ipȱʡ��"
							   "ֻ�޸����ö���������������"
					
					   "\n\n  -ip, --input �����ļ���������ʹ����-sѡ�����ȱʡ�����³���"
					   "\n  -if, --inputformat �����ļ���ʽ��ȱʡʱ�����Զ��ж�"
					   "\n  -op, --output ����ļ�����ȱʡʱĬ��Ϊ \"�����ļ���.����ļ���ʽ\""
					   "\n  -of, --outputformat ����ļ���ʽ��ȱʡʱĬ��Ϊass"
					   "\n  -oe, --outputencoding ����ļ����룬ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -t, --timeshift ʱ��ƫ������ȱʡʱĬ��Ϊ0.00"
					
					   "\n\n  -rx, --resx �ֱ��ʿ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -ry, --resy �ֱ��ʸߣ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -rt, --rolltime ������Ļ����ʱ�䣬ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -ht, --holdtime �̶���Ļ���ʱ�䣬ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -d, --density ��Ļ�ܶȣ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -fs, --fontsize ���ִ�С��ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -fn, --fontname �������ƣ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -o, --opacity ��͸���ȣ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -l, --outline ��̶߳ȣ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -sd, --shadow ��Ӱ�̶ȣ�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -b, --blank �ײ����ף�ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -bm, --blockmode ����ģʽ��ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n  -dm, --debugmode ����ģʽ��ȱʡʱ��ʹ�������ļ��е�ֵ"
					   "\n\n> ע��"
					   "\n  ����������пո���ʹ��˫���ţ����򽫻�ضϡ��磺\"Microsoft YaHei\""
					   "\n  ����������и�����ʹ�����ţ����򽫽���Ϊѡ��磺(-1024.25)"
					   "\n\n> ����"
					   "\n  danmakuFactory -ip \"D:/test.xml\"");
				exit(0); 
			}
			else if (strcmp(argv[cnt], "-s") == 0 || strcmp(argv[cnt], "--set") == 0)
			{/*����*/
				if (argc == 2)
				{
					printf("\n> ������");
					printf("\n  %10s �ֱ��ʿ� = %d", "resX", defaultConfig.resX);
					printf("\n  %10s �ֱ��ʸ� = %d", "resY", defaultConfig.resY);
					printf("\n");
					printf("\n  %10s ������Ļ�ٶȣ��룩 = %d", "rollTime", defaultConfig.rollTime);
					printf("\n  %10s �̶���Ļ���ʱ�䣨�룩 = %d", "holdTime", defaultConfig.holdTime);
					printf("\n  %10s ��Ļ�ܶȣ����� = %d", "density", defaultConfig.density);
					printf("\n");
					if (defaultConfig.outputEncoding == UTF_8)
					{
						printf("\n  %10s ������� = utf-8", "outputEncoding");
					}
					else
					{
						printf("\n  %10s ������� = ansi", "outputEncoding");
					}
					printf("\n  %10s ���ִ�С = %d", "fontSize", defaultConfig.fontSize);
					
					
					if (isUtf8(defaultConfig.fontName) == TRUE)
					{/*������˵���ı�*/
						transcoding(UTF_8, ANSI, defaultConfig.fontName, NULL, 0);
					}
					printf("\n  %10s ���� = %s", "fontName", defaultConfig.fontName);
					
					printf("\n  %10s ��͸���ȣ�0-255�� = %d", "opacity", defaultConfig.opacity);
					printf("\n  %10s ��ߣ�0-4�� = %d", "outline", defaultConfig.outline);
					printf("\n  %10s ��Ӱ��0-4�� = %d", "shadow", defaultConfig.shadow);
					printf("\n");
					printf("\n  %10s �ײ����ף����أ� = %d", "blank", defaultConfig.blank);
					printf("\n  %10s ���������Σ�����/����/����/�ײ�/����/��ɫ/�ظ��� = %s",
							"blockMode", defaultConfig.blockMode);
					printf("\n  %10s ����ģʽ�����ݱ��/�ֲ�ͼ�� = %s",
							"debugMode", defaultConfig.debugMode);
					printf("\n �������� 1 ��ʾ��Ӧλ�������0 ��ʾ��Ӧλ����ر�");
					exit(0);
				}
				save = TRUE;
				continue; 
			}
			
			/*������ѡ��ѡ��*/
			if (cnt + 1 < argc)
			{/*�������Ƿ�ȱʡ*/ 
				if (argv[cnt + 1][0] == '-')
				{
					printf("> �����������޷�ʶ����߲�����", argv[cnt]);
					printf("\n\n> ���÷���");
					printf("\n  ./danmakuFactory [-h][-s][-ip �����ļ���][-if �����ļ���ʽ][-op ����ļ���]"
								"[-of ����ļ���ʽ][-oe ����ļ�����][-t ʱ��ƫ��][-rx �ֱ��ʿ�][-ry �ֱ��ʸ�]"
								"[-rt ������Ļ����ʱ��][-ht �̶���Ļ���ʱ��][-d ��Ļ�ܶ�]"
								"[-fs ���ִ�С][-fn ����][-o ��͸����][-l ���][-sd ��Ӱ]"
								"[-b �ײ�����][-bm ����ģʽ][-dm ����ģʽ]");
					printf("\n\n ./danmakuFactory -h �鿴��ϸ");
					exit(1);
				}
			}
			else
			{
				printf("> �����������޷�ʶ����߲�����", argv[cnt]);
				printf("\n\n> ���÷���");
				printf("\n  ./danmakuFactory [-h][-s][-ip �����ļ���][-if �����ļ���ʽ][-op ����ļ���]"
							"[-of ����ļ���ʽ][-oe ����ļ�����][-t ʱ��ƫ��][-rx �ֱ��ʿ�][-ry �ֱ��ʸ�]"
							"[-rt ������Ļ����ʱ��][-ht �̶���Ļ���ʱ��][-d ��Ļ�ܶ�]"
							"[-fs ���ִ�С][-fn ����][-o ��͸����][-l ���][-sd ��Ӱ]"
							"[-b �ײ�����][-bm ����ģʽ][-dm ����ģʽ]");
				printf("\n\n ./danmakuFactory -h �鿴��ϸ");
				exit(1);
			}
			
			if (strcmp(argv[cnt], "-ip") == 0 || strcmp(argv[cnt], "--input") == 0)
			{/*�����ļ���*/
				strcpy(inputFileName, argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-if") == 0 || strcmp(argv[cnt], "--inputformat") == 0)
			{/*�����ļ���ʽ*/
				strcpy(inputFormat, argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-op") == 0 || strcmp(argv[cnt], "--output") == 0)
			{/*����ļ���*/
				strcpy(outputFileName, argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-of") == 0 || strcmp(argv[cnt], "--outputformat") == 0)
			{/*����ļ���ʽ*/
				strcpy(inputFormat, argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-oe") == 0 || strcmp(argv[cnt], "--outputencoding") == 0)
			{/*����ļ�����*/
				if (strcmp(toLower(NULL, argv[cnt + 1]), "utf-8") == 0)
				{
					defaultConfig.outputEncoding = UTF_8;
				}
				else if (strcmp(toLower(NULL, argv[cnt + 1]), "ansi") == 0)
				{
					defaultConfig.outputEncoding = ANSI;
				}
				else
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ��ѡֵĿǰֻ��utf-8��ansi");
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-t") == 0 || strcmp(argv[cnt], "--timeshift") == 0)
			{/*ʱ��ƫ��*/
				if (!isDesignatedChar(argv[cnt + 1], "()-.0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ����Ϊ�зǷ��ַ�", argv[cnt]);
					exit(1);
				}
				timeShift = strToFloat(argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-rx") == 0 || strcmp(argv[cnt], "--resx") == 0)
			{/*�ֱ��ʿ�*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ���ֱ��ʿ�����Ǵ���0������", argv[cnt]);
					exit(1);
				}
				defaultConfig.resX = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.resX <= 0)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �ֱ��ʿ�����Ǵ���0������");
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-ry") == 0 || strcmp(argv[cnt], "--resy") == 0)
			{/*�ֱ��ʸ�*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ���ֱ��ʸ߱����Ǵ���0������", argv[cnt]);
					exit(1);
				}
				defaultConfig.resY = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.resY <= 0)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �ֱ��ʸ߱����Ǵ���0������");
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-rt") == 0 || strcmp(argv[cnt], "--rolltime") == 0)
			{/*������Ļ�����ٶ�*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ��������Ļ�����ٶȱ����Ǵ���0������", argv[cnt]);
					exit(1);
				}
				defaultConfig.rollTime = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.rollTime <= 0)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ������Ļ�����ٶȱ����Ǵ���0������");
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-ht") == 0 || strcmp(argv[cnt], "--holdtime") == 0)
			{/*�̶���Ļ���ʱ��*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ���̶���Ļ���ʱ������Ǵ���0������", argv[cnt]);
					exit(1);
				}
				defaultConfig.holdTime = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.holdTime <= 0)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �̶���Ļ���ʱ������Ǵ���0������");
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-d") == 0 || strcmp(argv[cnt], "--density") == 0)
			{/*��Ļ�ܶ�*/
				if (!isDesignatedChar(argv[cnt + 1], "()-.0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ����Ϊ�зǷ��ַ�", argv[cnt]);
					exit(1);
				}
				defaultConfig.density = strToFloat(argv[cnt + 1]);
				if (defaultConfig.density < -1)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ��Ļ�ܶȱ����Ǵ��ڻ����-1������");
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-fs") == 0 || strcmp(argv[cnt], "--fontsize") == 0)
			{/*���ִ�С*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s �����ִ�С�����Ǵ���0������", argv[cnt]);
					exit(1);
				}
				defaultConfig.fontSize = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.fontSize <= 0)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ���ִ�С�����Ǵ���0������"); 
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-fn") == 0 || strcmp(argv[cnt], "--fontname") == 0)
			{/*��������*/
				strcpy(defaultConfig.fontName, argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-o") == 0 || strcmp(argv[cnt], "--opacity") == 0)
			{/*��͸����*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ����͸���ȱ�����0 - 255������", argv[cnt]);
					exit(1);
				}
				defaultConfig.opacity = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.opacity < 0 || defaultConfig.opacity > 255) 
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ��͸���ȱ�����0 - 255������"); 
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-l") == 0 || strcmp(argv[cnt], "--outline") == 0)
			{/*��͸����*/
				if (!isDesignatedChar(argv[cnt + 1], "()01234"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ����߱�����0 - 4������", argv[cnt]);
					exit(1);
				}
				defaultConfig.outline = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.outline < 0 || defaultConfig.outline > 4)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ��߱�����0 - 4������"); 
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-sd") == 0 || strcmp(argv[cnt], "--shadow") == 0)
			{/*��͸����*/
				if (!isDesignatedChar(argv[cnt + 1], "()01234"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ����Ӱ������0 - 4������", argv[cnt]);
					exit(1);
				}
				defaultConfig.shadow = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.shadow < 0 || defaultConfig.shadow > 4)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ��Ӱ������0 - 4������"); 
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-b") == 0 || strcmp(argv[cnt], "--blank") == 0)
			{/*�ײ�����*/
				if (!isDesignatedChar(argv[cnt + 1], "()0123456789"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ���ֱ��ʸ�Ϊ%dʱ���ײ����ױ�����0 - %d������",
							argv[cnt], defaultConfig.resY, defaultConfig.resY - defaultConfig.fontSize);
					exit(1);
				}
				defaultConfig.shadow = (int)(strToFloat(argv[cnt + 1]));
				if (defaultConfig.shadow < 0 ||
					defaultConfig.shadow > defaultConfig.resY - defaultConfig.fontSize)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �ֱ��ʸ�Ϊ%dʱ���ײ����ױ�����0 - %d������", 
							defaultConfig.resY, defaultConfig.resY - defaultConfig.fontSize);
					exit(1);
				}
			}
			else if (strcmp(argv[cnt], "-bm") == 0 || strcmp(argv[cnt], "--blockmode") == 0)
			{/*����ģʽ*/
				if (!isDesignatedChar(argv[cnt + 1], "()01"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ������ģʽֵֻ��Ϊ 0 �� 1",
							argv[cnt], defaultConfig.resY, defaultConfig.resY - defaultConfig.fontSize);
					exit(1);
				}
				if (strlen(argv[cnt + 1]) != 7)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ����ֻ����7��0��1���",
							argv[cnt], defaultConfig.resY, defaultConfig.resY - defaultConfig.fontSize);
					exit(1);
				}
				strcpy(defaultConfig.blockMode, argv[cnt + 1]);
			}
			else if (strcmp(argv[cnt], "-dm") == 0 || strcmp(argv[cnt], "--debugmode") == 0)
			{/*����ģʽ*/
				if (!isDesignatedChar(argv[cnt + 1], "()01"))
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        �����޷�����ֵ %s ������ģʽֵֻ��Ϊ 0 �� 1",
							argv[cnt], defaultConfig.resY, defaultConfig.resY - defaultConfig.fontSize);
					exit(1);
				}
				if (strlen(argv[cnt + 1]) != 2)
				{
					printf("> ����ѡ��%s�Ĳ�������", argv[cnt]);
					printf("\n        ����ֻ����2��0��1���",
							argv[cnt], defaultConfig.resY, defaultConfig.resY - defaultConfig.fontSize);
					exit(1);
				}
				strcpy(defaultConfig.debugMode, argv[cnt + 1]);
			}
			else
			{
				printf("> �����������޷�ʶ�������");
				printf("\n        �޷�ʶ�� %s", argv[cnt]);
				printf("\n\n> ���÷���");
				printf("\n  ./danmakuFactory [-h][-s][-ip �����ļ���][-if �����ļ���ʽ][-op ����ļ���]"
							"[-of ����ļ���ʽ][-oe ����ļ�����][-t ʱ��ƫ��][-rx �ֱ��ʿ�][-ry �ֱ��ʸ�]"
							"[-rt ������Ļ����ʱ��][-ht �̶���Ļ���ʱ��][-d ��Ļ�ܶ�]"
							"[-fs ���ִ�С][-fn ����][-o ��͸����][-l ���][-sd ��Ӱ]"
							"[-b �ײ�����][-bm ����ģʽ][-dm ����ģʽ]");
				printf("\n\n ./danmakuFactory -h �鿴��ϸ");
				exit(1);
			}
		}
		
		/*д�����ļ�*/
		if (save == TRUE)
		{
			if (!writeConfigFile(defaultConfig, configFilePath))
			{
				printf("\n> ���棺�����޷����µ��ļ�����Ϊ�����ļ���ʧ�򲻿�д\n");
			}
		}
		
		if (inputFileName[0] == '\0')
		{
			exit(0);
		} 
		
		/*ת�� xml ת ass*/
		if (outputFileName[0] == '\0')
		{
			sprintf(outputFileName, "%s.ass", inputFileName);
		}
		
		if(!access(outputFileName, 0))
		{
			printf("\n����ļ�%s�Ѵ��ڣ�����ִ��ԭ�ļ����ᱻ����\n", outputFileName);
			printf("����y��Y���������������ַ��˳�\n");
			fflush(stdin);
			char ch = getchar();
			if(ch != 'Y' && ch != 'y')
			{
				exit(0);
			}
		}
		
		int rt = 0;
		char code[] = "000";
		DANMAKU *danmakuPoorHead = NULL;
		clock_t startTime, endTime;
		
		printf("\n> ת����ʼ");
		startTime = clock();
		
		
		code[0] += readXml(inputFileName, &danmakuPoorHead, "n", timeShift);
		
		/*����ת��*/ 
		if (defaultConfig.outputEncoding == UTF_8 && isUtf8(defaultConfig.fontName) == FALSE)
		{/*������˵���ı�*/
			transcoding(ANSI, UTF_8, defaultConfig.fontName, NULL, 0);
		}
		else if (defaultConfig.outputEncoding == ANSI && isUtf8(defaultConfig.fontName) == TRUE)
		{
			transcoding(UTF_8, ANSI, defaultConfig.fontName, NULL, 0);
		}
		transListCoding(danmakuPoorHead, defaultConfig.outputEncoding);/*��ȫ����Ļ*/
		
		code[1] += sortList(&danmakuPoorHead);
		code[2] += writeAss(outputFileName,/*����ļ�*/
						    danmakuPoorHead,/*ͷָ��*/
						    defaultConfig.resX, defaultConfig.resY,/*�ֱ��ʿ�/�ֱ��ʸ�*/
						    defaultConfig.fontSize, defaultConfig.fontName,/*�ֺ�/����(utf-8)*/
						    defaultConfig.shadow, defaultConfig.outline,/*��Ӱ(0-4)/���(0-4)*/
						    defaultConfig.rollTime, defaultConfig.holdTime,/*�����ٶȣ�������Ļ��/ͣ��ʱ�䣨������Ļ��*/ 
						    defaultConfig.density, defaultConfig.opacity,/*�ܶ�/��͸����*/
						    defaultConfig.blank, /*�²�����*/
						    defaultConfig.blockMode,/*���Σ�����/����/����/�ײ�/����/��ɫ/�ظ���*/
						    defaultConfig.debugMode);/*����ģʽ������ͳ��/�ֲ�ͼ��*/
		endTime = clock();
		
		if (strcmp(code, "000") == 0 || strcmp(code, "009") == 0)
		{
			printf("-> ת���ɹ�");
			printf(" ��ʱ %d ms ״̬���� C%s", endTime - startTime, code);
		}
		else
		{
			printf("-> ת��ʧ��");
			printf(" ��ʱ %d ms ״̬���� C%s ", endTime - startTime, code);
			printErrInfo(code);
		}
		
		
		printf("\n����ļ�Ϊ %s", outputFileName);
		printf("\nMade by TIKM");
		printf("\nhttps://github.com/HITIKM/DanmakuFactory");
		endTime = clock();
		exit(0);
	}
	return 0;
}

/*
�������ļ�
������
������Ϣ�ṹ��ָ��/�����ļ���/
*/ 
void readConfigFile(CONFIG *config, char *fileName)
{
	BOOL noErr = TRUE;
	char tempText[MAX_TEXT_LENGTH];
	
	(*config).resX = GetPrivateProfileInt("DanmakuSet", "resX", 1920, fileName);
	(*config).resY = GetPrivateProfileInt("DanmakuSet", "resY", 1080, fileName);
	(*config).fontSize = GetPrivateProfileInt("DanmakuSet", "fontSize", 38, fileName);
	(*config).shadow = GetPrivateProfileInt("DanmakuSet", "shadow", 1, fileName);
	(*config).outline = GetPrivateProfileInt("DanmakuSet", "outline", 0, fileName);
	(*config).rollTime = GetPrivateProfileInt("DanmakuSet", "rollTime", 10, fileName);
	(*config).holdTime = GetPrivateProfileInt("DanmakuSet", "holdTime", 10, fileName);
	(*config).density = GetPrivateProfileInt("DanmakuSet", "density", 0, fileName);
	(*config).opacity = GetPrivateProfileInt("DanmakuSet", "opacity", 180, fileName);
	(*config).blank = GetPrivateProfileInt("DanmakuSet", "blank", 0, fileName);
	GetPrivateProfileString("DanmakuSet", "fontName", "Microsoft YaHei Light", (*config).fontName,
							MAX_TEXT_LENGTH, fileName);
	 GetPrivateProfileString("DanmakuSet", "opEncoding", "UTF-8", tempText, MAX_TEXT_LENGTH, fileName);					
	if (strcmp(toLower(NULL, tempText), "utf-8") == 0)
	{
		(*config).outputEncoding = UTF_8;
	}
	else
	{
		(*config).outputEncoding = ANSI;
	}
	
	GetPrivateProfileString("DanmakuSet", "blockR2L", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[0] = '1';
	}
	else
	{
		(*config).blockMode[0] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "blockL2R", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[1] = '1';
	}
	else
	{
		(*config).blockMode[1] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "blockTop", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[2] = '1';
	}
	else
	{
		(*config).blockMode[2] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "blockBottom", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[3] = '1';
	}
	else
	{
		(*config).blockMode[3] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "blockSpecial", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[4] = '1';
	}
	else
	{
		(*config).blockMode[4] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "blockColor", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[5] = '1';
	}
	else
	{
		(*config).blockMode[5] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "blockRepeat", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).blockMode[6] = '1';
	}
	else
	{
		(*config).blockMode[6] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "debugTable", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).debugMode[0] = '1';
	}
	else
	{
		(*config).debugMode[0] = '0';
	}
	
	GetPrivateProfileString("DanmakuSet", "debugChart", "false", tempText, MAX_TEXT_LENGTH, fileName);
	if (strcmp(toLower(NULL, tempText), "true") == 0)
	{
		(*config).debugMode[1] = '1';
	}
	else
	{
		(*config).debugMode[1] = '0';
	}
}

/*
д�����ļ�
������
������Ϣ�ṹ��/�����ļ���/
�ɹ�����TRUE ʧ�ܷ���FALSE
*/ 
BOOL writeConfigFile(CONFIG config, char *fileName)
{
	BOOL noErr = TRUE;
	char tempText[MAX_TEXT_LENGTH];
	
	sprintf(tempText, "%d", config.resX);
	noErr &= WritePrivateProfileString("DanmakuSet", "resX", tempText, fileName);
	
	sprintf(tempText, "%d", config.resY);
	noErr &= WritePrivateProfileString("DanmakuSet", "resY", tempText, fileName);
	
	sprintf(tempText, "%d", config.rollTime);
	noErr &= WritePrivateProfileString("DanmakuSet", "RollTime", tempText, fileName);
	
	sprintf(tempText, "%d", config.holdTime);
	noErr &= WritePrivateProfileString("DanmakuSet", "holdTime", tempText, fileName);
	
	sprintf(tempText, "%d", config.shadow);
	noErr &= WritePrivateProfileString("DanmakuSet", "shadow", tempText, fileName);
	
	sprintf(tempText, "%d", config.outline);
	noErr &= WritePrivateProfileString("DanmakuSet", "outline", tempText, fileName);
	
	sprintf(tempText, "%d", config.density);
	noErr &= WritePrivateProfileString("DanmakuSet", "density", tempText, fileName);
	
	sprintf(tempText, "%d", config.opacity);
	noErr &= WritePrivateProfileString("DanmakuSet", "opacity", tempText, fileName);
	
	sprintf(tempText, "%d", config.blank);
	noErr &= WritePrivateProfileString("DanmakuSet", "blank", tempText, fileName);
	
	sprintf(tempText, "%d", config.fontSize);
	noErr &= WritePrivateProfileString("DanmakuSet", "fontSize", tempText, fileName);
	
	noErr &= WritePrivateProfileString("DanmakuSet", "fontName", config.fontName, fileName);
	
	if (config.blockMode[0] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockR2L", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockR2L", "false", fileName);
	}
	
	if (config.blockMode[1] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockL2R", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockL2R", "false", fileName);
	}
	
	if (config.blockMode[2] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockTop", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockTop", "false", fileName);
	}
	
	if (config.blockMode[3] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockBottom", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockBottom", "false", fileName);
	}
	
	if (config.blockMode[4] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockSpecial", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockSpecial", "false", fileName);
	}
	
	if (config.blockMode[5] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockColor", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockColor", "false", fileName);
	}
	
	if (config.blockMode[6] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockRepeat", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "blockRepeat", "false", fileName);
	}
	
	if (config.debugMode[0] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "debugTable", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "debugTable", "false", fileName);
	}
	
	if (config.debugMode[1] != '0')
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "debugChart", "true", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "debugChart", "false", fileName);
	}
	
	if (config.outputEncoding != UTF_8)
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "opEncoding", "ansi", fileName);
	}
	else
	{
		noErr &= WritePrivateProfileString("DanmakuSet", "opEncoding", "utf-8", fileName);
	}
	
	return noErr;
}

/*
��ӡת������ԭ�� 
������
״̬�� 
*/
void printErrInfo(char *code)
{
	if (strcmp(code, "000") == 0 || strcmp(code, "009") == 0)
	{
		printf("����");
	}
	else if (code[0] == '1')
	{
		printf("���ļ�ʧ��");
	}
	else if (code[0] == '2' || code[0] == '3' || code[0] == '4')
	{
		printf("���ļ�ʱ��������");
	}
	else if (code[0] == '5' || code[0] == '6' || code[1] == '2' || (code[2] >= '3' && code[2] <= '7'))
	{
		printf("�ڴ������������");
	}
	else if (code[0] == '7')
	{
		printf("�ļ�δ�ܰ���ȷ��ʽ����");
	}
	else if (code[2] == '2')
	{
		printf("�����򸲸��ļ�ʧ��");
	}
	else if (code[2] == '8')
	{
		printf("д�ļ�ʱ��������");
	}
}
