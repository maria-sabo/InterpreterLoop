#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "ilist.h"

int load_prog(FILE *, DblLinkedList * ); 
int load_var(FILE *, DblLinkedList * );

// разбиваем программу через ; так чтобы было через \n в list_prog 
int load_prog(FILE *file_prog, DblLinkedList * list_prog) {
		int i = 0;
		char *buffer1; // строка до ;
		char *buffer2; // LOOPx хранится в буфере
		char *buffer3; // DO
		char *buffer4; // остаток строки -> в первый и опять укорачивание 
		char *buf;
		char ch;
		char *yk;

		buffer1 = (char *)malloc(1 * sizeof(char)); 
		buffer2 = (char *)malloc(1 * sizeof(char)); 
		buffer3 = (char *)malloc(3 * sizeof(char));
		buffer4 = (char *)malloc(1 * sizeof(char));

		//int lencount = 0;

		while ((ch = fgetc(file_prog)) != EOF) {
			if (!(ch == ' ' || ch == '\n' || ch == '\t')) {
				if (ch == ';') {
					buffer1 = realloc(buffer1, sizeof(char) * (i + 1));
					if (!checkMemory(buffer1)) return 0;
					buffer1[i] = '\0'; // строка кончилась 
					int ex = 1;
					// несколько LOOP DO подряд
					while (ex) {
						yk = strstr(buffer1, "DO");
						if (yk == NULL) {
							pushBack(list_prog, (char *)buffer1);
							buffer1 = (char *)malloc(1 * sizeof(char));
							ex = 0;
						}
						else {
							buffer2 = realloc(buffer2, sizeof(char) * (yk - buffer1 + 1));
							if (buffer2 == NULL) {
								printf("Memory allocation error \n");
								return 0;
							}
							strncpy(buffer2, buffer1, yk - buffer1);
							buffer2[yk - buffer1] = '\0';
							pushBack(list_prog, (char *)buffer2);
							buffer2 = (char *)malloc(1 * sizeof(char));
							
							strcpy(buffer3, "DO\0");
							pushBack(list_prog, (char *)buffer3);
							buffer3 = (char *)malloc(3 * sizeof(char));

							buffer4 = realloc(buffer4, sizeof(char) * (strlen(yk + 2) + 1));
							if (buffer4 == NULL) {
								printf("Memory allocation error \n");
								return 0;
							}
							strcpy(buffer4, yk + 2);
							buf = strstr(buffer4, "DO");
							if (buf == NULL) {
								pushBack(list_prog, (char *)buffer4);
								buffer4 = (char *)malloc(1 * sizeof(char));
								ex = 0;
							}
							else {
								buffer1 = realloc(buffer1, sizeof(char) *(strlen(buffer4) + 1));
								if (buffer1 == NULL) {
									printf("Memory allocation error \n");
									return 0;
								}
								strcpy(buffer1, buffer4);
							}
						}
					}
					i = 0;
				}
				else {
					buffer1[i] = ch;
					i++;
					buffer1 = realloc(buffer1, sizeof(char) * (i + 1));
					if (buffer1 == NULL) {
						printf("Memory allocation error \n");
						return 0;
					}
				}
			}
		}
		int exit = 1;
		buffer1[i] = '\0';
		free(buffer1);
		free(buffer4);
		buffer1 = NULL;
		buffer4 = NULL;
		buffer2 = NULL;
		buffer3 = NULL;
		
		//system("cls");
		// печатаем список команд 
		if (exit)
			printDblLinkedList(list_prog);
		//system("pause");
		printf("Enter a key to continue...\n");
		getchar();
		return exit;
	//system("pause");
	printf("Enter a key to continue...\n");
	getchar();
	return 1;
}
// разбиваем программу через ; и , так чтобы было \n в list_var
int load_var(FILE *file_var, DblLinkedList * list_var) {
	//FILE *file_var;
	//file_var = fopen(varName, "r");
		int i = 0;
		char *buffer1;
		//int lencount = 0;
		char ch;
		buffer1 = (char *)malloc(1 * sizeof(char));
		while ((ch = fgetc(file_var)) != EOF) {
			if (!(ch == ' ' || ch == '\n' || ch == '\t')) {
				if (ch == ';' || ch == ',') {
					buffer1 = realloc(buffer1, sizeof(char) * (i + 1));
					if (buffer1 == NULL) {
						printf("Memory allocation error \n");
						return 0;
					}
					buffer1[i] = '\0';
					pushBack(list_var, (char *)buffer1);
					buffer1 = (char *)malloc(1 * sizeof(char));
					i = 0;
				}
				else {
					buffer1[i] = ch;
					i++;
					buffer1 = realloc(buffer1, sizeof(char) *(i + 1));
					if (buffer1 == NULL) {
						printf("Memory allocation error \n");
						return 0;
					}
				}
			}
		}
		buffer1[i] = '\0';

		//system("cls");
		// печатаем список переменных 
		printDblLinkedList(list_var);
		//system("pause");
		printf("Enter a key to continue...\n");
		getchar();
		return 1;
	//system("pause");
	printf("Enter a key to continue...\n");
	getchar();
	return 1;
}
