#include "stdafx.h"
#include "ilist.h"
#include "command.h"

int checkpline(char*, int, DblLinkedList *, int*, int*, int*);
int checkvline(char*, int, DblLinkedList *);

int checkSyntax(FILE* file_out, DblLinkedList * list_prog, DblLinkedList * list_var, 
	DblLinkedList * list_command, DblLinkedList * list_init) {

	// �������� ��������c� ���������
	int k_end, k_do, k_loop; // ���������� end, do, loop � ���������
	k_end = 0;
	k_do = 0;
	k_loop = 0;

	int okp = 0; // ��� �������� �������� ������ ���������
	int ns = 0; // ����� ������ 
	void *buf;
	buf = popFront(list_prog);
	while (buf != NULL) {
		ns++;
		okp = okp + checkpline((char *)buf, ns, list_command, &k_loop, &k_do, &k_end);
		buf = popFront(list_prog);
	}

	printCommandList(list_command, file_out);

	printf("Programm: %d syntax error found\n", okp);
	if (file_out != NULL)
		fprintf(file_out, "Programm: %d syntax error found\n", okp);
	if (k_end != k_loop || k_do != k_loop || k_end != k_do) {
		okp++;
		printf("Programm: %d LOOP - %d DO - %d END error found\n", k_loop, k_do, k_end);
		if (file_out != NULL)
			fprintf(file_out, "Programm: %d LOOP - %d DO - %d END error found\n", k_loop, k_do, k_end);
	}

	printf("Enter a key to continue...\n");
	getchar();

	// �������� ���������� ����������
	ns = 0;
	int okv = 0; // ��� �������� �������� ������ ����������
	buf = popFront(list_var);
	while (buf != NULL) {
		ns++;
		okv = okv + checkvline((char *)buf, ns, list_init);
		buf = popFront(list_var);
	}
	printCommandList(list_init, file_out);
	printf("Variable: %d syntax error found\n", okv);
	if (file_out != NULL)
		fprintf(file_out, "Variable: %d syntax error found\n", okv);
	printf("Enter a key to continue...\n");
	getchar();

	if (okp + okv == 0)
		return 1;
	else
		return 0;
}


// �������� ������ ���������
int checkpline(char* b, int ns, DblLinkedList * list_command, int * c_loop, int * c_do, int * c_end) {
	char * yk;
	Command * com;

	int co, er, p1 = 0, p2 = 0, p3 = 0; // �o - ����� �������: 1 - DO, 2 - END, 3 - LOOPx, 4 - :=, 5 - :=+, 6 - :=-
	                                    // er - ����� ������
										// p1 - ������ ���������� ����� ��� ���������� � LOOP
										// p2 - ������ ���������� ������ ��� ��������� 
										// p3 - ���������
	int ok1 = -1;
	int ok2 = -1;
	int ok3 = -1;
	int ok4 = -1;
	int of;
	yk = strstr(b, "DO"); // DO ������ � ������
	if (yk != NULL) {
		*c_do = *c_do + 1;
		if (strlen(b) == 2) { // 2 - ����� DO
			if (*c_do > *c_loop) { 
				co = 1; er = 14; // ���������� DO ������������
				ok1 = 1;
			}
			else {
				co = 1;	er = 0;
				ok1 = 1;
			}
		}
		else { 
			co = 1;	er = 1; // ������������ ��������� ������� DO
			ok1 = 0;
		}
	}

	if (ok1 == -1) {
		yk = strstr(b, "END"); // END ������ � ������
		if (yk != NULL) {
			*c_end = *c_end + 1;
			if (strlen(b) == 3) { // 3 - ����� END
				if (*c_end > *c_loop || *c_end > *c_do) { 
					co = 2; er = 15; // ���������� END ������������
					ok2 = 0;
				}
				else {
					co = 2;	er = 0;
					ok2 = 1;
				}
			}
			else {
				co = 2;	er = 2; // ������������ ��������� ������� END
				ok2 = 0;
			}
		}

		if (ok2 == -1 && ok1 == -1) {
			yk = strstr(b, "LOOPx"); // LOOPx � ������ 
			if (yk != NULL) {
				*c_loop = * c_loop + 1;
				if (yk - b != 0 || strlen(b) == 5) { // 5 - ����� LOOPx
					co = 3;	er = 3; // ����� LOOPx ������������ ������
					ok3 = 0;
				}
				else {
					of = 1;
					for (size_t i = 5; i < strlen(b); i++) {  
						if ((int)b[i] < 48 || (int)b[i] > 57) { // ����� � ����� �����, ��������� �� �� ���� (�������� �� ASCII)
							of = 0;
						}
						if (!of) {
							co = 3;	er = 4; // ������ � ������� � ����� LOOP
							ok3 = 0;
						}
						else {
							p1 = atoi(yk + 5); // � p1 ����� ��������� ������ �
							if (p1 <= INT_MAX && p1 >= 0) {
								co = 3;	er = 0; 
								ok3 = 1;
							}
							else {
								co = 3; er = 24; // �������� ����� ��������
								ok3 = 0;
							}
							
						}
					}
				}
			}

			if (ok3 == -1 && ok2 == -1 && ok1 == -1) {
				yk = strstr(b, ":="); // := � ������
				if (yk != NULL) {
					int bsc = strlen(b);
					char *left = malloc(bsc * sizeof(char)); // ����� ����� ��������� xN:=xM+C, �.� xN
					if (!checkMemory(left)) return 0;
					char *right = malloc(bsc * sizeof(char)); // ������ ����� ��������� xN:=xM+C, �.� xM+C
					if (!checkMemory(right)) return 0;
					char *rleft = malloc(bsc * sizeof(char)); // xM
					if (!checkMemory(rleft)) return 0;
					char *rright = malloc(bsc * sizeof(char)); // C
					if (!checkMemory(rright)) return 0;
					strncpy(left, b, yk - b);
					left[yk - b] = '\0';
					strcpy(right, yk + 2); 
					yk = strstr(left, "x"); // x � ����� ����� ������
					if (yk == NULL || yk - left != 0 || strlen(yk) == 1) {
						co = 4;	er = 6; // ������������ ��������� ������� ����� ����� ����� :=
						ok4 = 0;
					}
					else {
						of = 1;
						for (size_t i = 1; i < strlen(left); i++) { 
							if ((int)left[i] < 48 || (int)left[i] > 57) { // ����� � �� ����� �����, ��������� �� ���� (�������� �� ASCII)
								of = 0;
							}
						}
						if (!of) {
							co = 4;	er = 7; // ������ � ������� � ����� :=
							ok4 = 0;
						}
						else {
							p1 = atoi(left + 1); // � p1 ����� ��������� �����c �
							if (p1 > INT_MAX || p1 < 0) {
								co = 4; er = 24;
								ok4 = 0;
							}
						}
					}
					if(ok4 != 0) {

						yk = strstr(right, "+"); // + � ������ ����� ������
						int plus = 1;
						if (yk == NULL) {
							yk = strstr(right, "-"); // - � ������ ����� ������
							plus = 0;
						}
						if (yk == NULL) {
						co = 4;	er = 8; // �� ������ +/-
						ok4 = 0;
						}
						else {
							strncpy(rleft, right, yk - right);
							rleft[yk - right] = '\0';
							strcpy(rright, yk + 1);
							yk = strstr(rleft, "x");
							if (yk == NULL || yk - rleft != 0 || strlen(yk) == 1) {
								co = 4;	er = 5; // ������������ ��������� ������� ����� ����� ����� +-
								ok4 = 0;
							}
							else {
								of = 1;
								for (size_t i = 1; i < strlen(rleft); i++) 
									if ((int)rleft[i] < 48 || (int)rleft[i] > 57) // ����� � �� ����� �����, ��������� �� ���� (�������� �� ASCII)
										of = 0;
								if (!of) {
									co = 4;	er = 5; // ������������ ��������� ������� ����� ����� ����� +-
									ok4 = 0;
								}
								else {
									p2 = atoi(rleft + 1); // � p2 ����� ��������� ������ �M  
									if (p2 > INT_MAX || p2 < 0) {
										co = 4; er = 24;
										ok4 = 0;
									}
									else {
										of = 1;
										int a = strlen(rright);
										for (int i = 0; i < a - 1; i++)
											if ((int)rright[i] < 48 || (int)rright[i] > 57)
												of = 0;
										if (!of) {
											co = 4;	er = 23; // ������������ ��������� ������� ����� ����� ����� +-
											ok4 = 0;
										}
										else {
											if (plus)
												co = 4;
											else
												co = 5;
											er = 0;
											p3 = atoi(rright); // � p3 ����� ��������� C
											ok4 = 1;
										}
									}
								}
							}
						}
					}
					free(left);
					free(right);
					free(rleft);
					free(rright);
				}
			}
		}
	}

	if (ok1 == -1 && ok2 == -1 && ok3 == -1 && ok4 == -1) {
		co = 7;	er = 9;	p1 = 0; p2 = 0; p3 = 0; ok4 = 0;
	}
	com = createCommand(ns, b, co, er, p1, p2, p3);
	pushBack(list_command, com);

	if (ok1 == 0 || ok2 == 0 || ok3 == 0 || ok4 == 0)
		return 1;
	else
		return 0;
}


// �������� ���������� ������ ����� ����������
int checkvline(char* b, int ns, DblLinkedList * list_init) {
	char * yk;
	Command * com;

	yk = strstr(b, "=");
	int co = 6; // �������, ������������, ��� ��� ������������� ����������
				// p1 - ������ ���������� ����� ��� ���������� � LOOP
				// p2 - ������ ���������� ������ ��� ��������� 
				// p3 - ���������
	int er = 0; // ����� ������
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int of; // ����
	if (yk != NULL) {
		int bsc = strlen(b);
		char *left = malloc(bsc * sizeof(char));
		if (!checkMemory(left)) return 0;
		char *right = malloc(bsc * sizeof(char));
		if (!checkMemory(right)) return 0;
		strncpy(left, b, yk - b); // xN, ��� xN=C
		left[yk - b] = '\0';
		strcpy(right, yk + 1); // �, ��� xN=C
		yk = strstr(left, "x");
		if (yk == NULL || yk - left != 0 || strlen(left) == 1) {
			co = 6;	er = 10; // ������������ ��������� � � ����� �����
		}
		else {
			of = 1;
			for (size_t i = 1; i < strlen(left); i++) {
				if ((int)left[i] < 48 || (int)left[i] > 57) { // ����� � �� ����� �����, ��������� �� ���� (�������� �� ASCII)
					of = 0;
				}
			}
			if (!of) {
				co = 6;	er = 11; // ����� � �� �������� ������� 
			}
			else {
				p1 = atoi(left + 1); // � p1 ����� ��������� ������ � � ����� ����� 
				if (p1 > INT_MAX || p1 < 0) {
					co = 6; er = 24; // ������ ��������
				}
				else {
					of = 1;
					for (size_t i = 1; i < strlen(right); i++)
						if ((int)right[i] < 48 || (int)right[i] > 57) // ����� � �� ����� �����, ��������� �� ���� (�������� �� ASCII)
							of = 0;
					if (!of) {
						co = 6;	er = 12; // ������������ �������� � ������ ����� 
					}
					else {
						p2 = atoi(right); // � p2 ����� ��������� ������ ����� 
						co = 6; er = 0;
					}
				}
			}
		}
		free(left);
		free(right);
	}
	else {
		co = 7; er = 13;
	}
	com = createCommand(ns, b, co, er, p1, p2, p3);
	pushBack(list_init, com);

	if (er == 0)
		return 0;
	else
		return 1;
}