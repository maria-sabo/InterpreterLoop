#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "ilist.h"

#define BSS 1000 // маскимальная длина строки программы
#define BSC 100 // максимальная длина команды 

int load_prog(char *, DblLinkedList * ); 
int load_var(char *, DblLinkedList * );

// разбиваем программу через ; так чтобы было через \n в list_prog 
int load_prog(char *progName, DblLinkedList * list_prog) {

	FILE *file_prog;
	file_prog = fopen(progName, "r");
	if (file_prog != NULL) {
		int i = 0;
		char *buffer5;
		char *buffer1;
		char *buffer2;
		char *buffer3;
		char *buffer4;
		char *buf;
		char ch;
		char *yk;

		buffer1 = (char *)malloc(BSS * sizeof(char));
		buffer2 = (char *)malloc(BSC * sizeof(char));
		buffer3 = (char *)malloc(BSC * sizeof(char));
		buffer4 = (char *)malloc(BSS * sizeof(char));
		buffer5 = (char *)malloc(BSC * sizeof(char));

		int lencount = 0;

		while ((ch = fgetc(file_prog)) != EOF) {
			if (!(ch == ' ' || ch == '\n' || ch == '\t')) {
				if (ch == ';') {
					buffer1[i] = '\0'; // строка кончилась 
					int ex = 1;
					// несколько LOOP DO подряд
					while (ex) {
						yk = strstr(buffer1, "DO");
						if (yk == NULL) {
							if (strlen(buffer1) <= BSC) {
								strcpy(buffer5, buffer1);
								pushBack(list_prog, (char *)buffer5);
								buffer5 = (char *)malloc(BSC * sizeof(char));
							}
							else {
								lencount ++;
								printf("Command is too long- %s\n", buffer1);
							}
							ex = 0;
						}
						else {
							strncpy(buffer2, buffer1, yk - buffer1);
							buffer2[yk - buffer1] = '\0';
							if (strlen(buffer2) <= BSC) {
								strcpy(buffer5, buffer2);
								pushBack(list_prog, (char *)buffer5);
								buffer5 = (char *)malloc(BSS * sizeof(char));
							}
							else {
								lencount++;
								printf("Command is too long - %s\n", buffer2);
							}

							strcpy(buffer3, "DO\0");
							pushBack(list_prog, (char *)buffer3);
							buffer3 = (char *)malloc(BSS * sizeof(char));

							strcpy(buffer4, yk + 2);
							buf = strstr(buffer4, "DO");
							if (buf == NULL) {
								if (strlen(buffer4) <= BSC) {
									strcpy(buffer5, buffer4);
									pushBack(list_prog, (char *)buffer5);
									buffer5 = (char *)malloc(BSS * sizeof(char));
								}
								else {
									lencount++;
									printf("Command is too long - %s\n", buffer4);
								}
								ex = 0;
							}
							else
								strcpy(buffer1, buffer4);
						}
					}
					i = 0;
				}
				else {
					buffer1[i] = ch;
					i++;
				}
				if (i > BSS - 1)
					break;
			}
		}
		buffer1[i] = '\0';
		if (i > BSS - 1) {
			printf("String is too long - %s\n", buffer1);
			system("pause");
			return 0;
		}
		if (lencount != 0) {
			printf("%d Command is too long\n", lencount);
			system("pause");
			return 0;
		}

		fclose(file_prog);
		system("cls");
		// печатаем список команд 
		printDblLinkedList(list_prog, printString);
		system("pause");
		return 1;
	}

	else
		printf("File %s is not found \n", progName);
	
	system("pause");
	return 0;
}
// разбиваем программу через ; и , так чтобы было \n в list_var
int load_var(char *varName, DblLinkedList * list_var) {

	FILE *file_var;
	file_var = fopen(varName, "r");
	if (file_var != NULL) {
		int i = 0;
		char *buffer1;
		int lencount = 0;
		char ch;
		buffer1 = (char *)malloc(BSC * sizeof(char));
		while ((ch = fgetc(file_var)) != EOF) {
			if (!(ch == ' ' || ch == '\n' || ch == '\t')) {
				if (ch == ';' || ch == ',') {
					buffer1[i] = '\0';
					if (strlen(buffer1) <= BSC) {
						pushBack(list_var, (char *)buffer1);
						buffer1 = (char *)malloc(BSC * sizeof(char));
					}
					else {
						printf("Command is too long - %s\n", buffer1);
						lencount++;
					}
					i = 0;
				}
				else {
					buffer1[i] = ch;
					i++;
				}
				if (i > BSS - 1)
					break;
			}
		}
		buffer1[i] = '\0';
		if (strlen(buffer1) <= BSC) {
			pushBack(list_var, (char *)buffer1);
		}
		else {
			printf("Command is too long - %s\n", buffer1);
			lencount++;
		}

		if (i > BSS - 1) {
			printf("String is too long - %s\n", buffer1);
			system("pause");
			return 0;
		}
		if (lencount != 0) {
			printf("%d Command is too long\n", lencount);
			system("pause");
			return 0;
		}


		fclose(file_var);
		system("cls");
		// печатаем список переменных 
		printDblLinkedList(list_var, printString);
		system("pause");
		return 1;
	}
	else
		printf("File %s is not found\n", varName);
	system("pause");
	return 0;
}
