#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "ilist.h"

int load_prog(FILE *, DblLinkedList * ); 
int load_var(FILE *, DblLinkedList * );

// ��������� ��������� ����� ; ��� ����� ���� ����� \n � list_prog 
int load_prog(FILE *file_prog, DblLinkedList * list_prog) {
		int i = 0;
		char *buffer1; // ������ �� ;
		char *buffer2; // LOOPx �������� � ������
		char *buffer3; // DO
		char *buffer4; // ������� ������ -> � ������ � ����� ������������ 
		char *buf;
		char ch;
		char *yk;

		buffer1 = (char *)malloc(1 * sizeof(char)); 
		buffer2 = (char *)malloc(1 * sizeof(char)); 
		buffer3 = (char *)malloc(3 * sizeof(char));
		buffer4 = (char *)malloc(1 * sizeof(char));


		while ((ch = fgetc(file_prog)) != EOF) {
			if (!(ch == ' ' || ch == '\n' || ch == '\t')) {
				if (ch == ';') {
					buffer1 = realloc(buffer1, sizeof(char) * (i + 10));
					if (!checkMemory(buffer1)) return 0;
					buffer1[i] = '\0'; // ������ ��������� 
					int ex = 1;
					// ��������� LOOP DO ������
					while (ex) {
						yk = strstr(buffer1, "DO");
						if (yk == NULL) {
							pushBack(list_prog, (char *)buffer1);
							buffer1 = (char *)malloc(1 * sizeof(char));
							if (!checkMemory(buffer1)) return 0;
							ex = 0;
						}
						else {
							buffer2 = realloc(buffer2, sizeof(char) * (yk - buffer1 + 10));
							if (!checkMemory(buffer2)) return 0;
							strncpy(buffer2, buffer1, yk - buffer1);
							buffer2[yk - buffer1] = '\0';
							pushBack(list_prog, (char *)buffer2);
							buffer2 = (char *)malloc(1 * sizeof(char));
							if (!checkMemory(buffer2)) return 0;
							
							strcpy(buffer3, "DO\0");
							pushBack(list_prog, (char *)buffer3);
							buffer3 = (char *)malloc(3 * sizeof(char));
							if (!checkMemory(buffer3)) return 0;

							buffer4 = realloc(buffer4, sizeof(char) * (strlen(yk + 2) + 10));
							if (!checkMemory(buffer4)) return 0;
							strcpy(buffer4, yk + 2);
							buf = strstr(buffer4, "DO");
							if (buf == NULL) {
								pushBack(list_prog, (char *)buffer4);
								buffer4 = (char *)malloc(1 * sizeof(char));
								if (!checkMemory(buffer4)) return 0;
								ex = 0;
							}
							else {
								buffer1 = realloc(buffer1, sizeof(char) *(strlen(buffer4) + 10));
								if (!checkMemory(buffer1)) return 0;
								strcpy(buffer1, buffer4);
							}
						}
					}
					i = 0;
				}
				else {
					buffer1[i] = ch;
					i++;
					buffer1 = realloc(buffer1, sizeof(char) * (i + 10));
					if (!checkMemory(buffer1)) return 0;
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
		
		// �������� ������ ������ 
		if (exit)
			printDblLinkedList(list_prog);
		printf("Enter a key to continue...\n");
		getchar();
		return exit;
	    printf("Enter a key to continue...\n");
	    getchar();
	    return 1;
}
// ��������� ��������� ����� ; � , ��� ����� ���� \n � list_var
int load_var(FILE *file_var, DblLinkedList * list_var) {
	int i = 0;
	char *buffer1;
	char ch;
	buffer1 = (char *)malloc(1 * sizeof(char));
	while ((ch = fgetc(file_var)) != EOF) {
		if (!(ch == ' ' || ch == '\n' || ch == '\t')) {
			if (ch == ';' || ch == ',') {
				buffer1 = realloc(buffer1, sizeof(char) * (i + 10));
				if (!checkMemory(buffer1)) return 0;
				buffer1[i] = '\0';
				pushBack(list_var, (char *)buffer1);
				buffer1 = (char *)malloc(1 * sizeof(char));
				i = 0;
			}
			else {
				buffer1[i] = ch;
				i++;
				buffer1 = realloc(buffer1, sizeof(char) *(i + 10));
				if (!checkMemory(buffer1)) return 0;
			}
		}
    }
	buffer1[i] = '\0';
	// �������� ������ ���������� 
	printDblLinkedList(list_var);
	printf("Enter a key to continue...\n");
	getchar();
	return 1;
    printf("Enter a key to continue...\n");
	getchar();
	return 1;
}
