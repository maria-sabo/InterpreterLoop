#include "stdafx.h"
#include "ilist.h"
#include "command.h"

#define INDLIMIT 2147483646 // определяем так максимальный индекс переменной 

int checkpline(char*, int, DblLinkedList *, int*, int*, int*);
int checkvline(char*, int, DblLinkedList *);

int checkSyntax(FILE* file_out, DblLinkedList * list_prog, DblLinkedList * list_var, 
	DblLinkedList * list_command, DblLinkedList * list_init) {

	// проверка синтаксиcа программы
	int k_end, k_do, k_loop;
	k_end = 0;
	k_do = 0;
	k_loop = 0;

	int okp = 0;
	int ns = 0;
	void *buf;
	buf = popFront(list_prog);
	while (buf != NULL) {
		ns++;
		okp = okp + checkpline((char *)buf, ns, list_command, &k_loop, &k_do, &k_end);
		buf = popFront(list_prog);
	}

	//system("cls");
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

	//system("pause");
	printf("Enter a key to continue...\n");
	getchar();

	// проверка синтаксиса переменных
	ns = 0;
	int okv = 0;
	buf = popFront(list_var);
	while (buf != NULL) {
		ns++;
		okv = okv + checkvline((char *)buf, ns, list_init);
		buf = popFront(list_var);
	}
	//system("cls");
	printCommandList(list_init, file_out);
	printf("Variable: %d syntax error found\n", okv);
	if (file_out != NULL)
		fprintf(file_out, "Variable: %d syntax error found\n", okv);
	//system("pause");
	printf("Enter a key to continue...\n");
	getchar();

	if (okp + okv == 0)
		return 1;
	else
		return 0;
}


// проверка строки программы
int checkpline(char* b, int ns, DblLinkedList * list_command, int * c_loop, int * c_do, int * c_end) {
	char * yk;
	//char * e;
	Command * com;
	//extern int k_do, k_end, k_loop;

	int co, er, p1 = 0, p2 = 0, p3 = 0;
	int ok1 = -1;
	int ok2 = -1;
	int ok3 = -1;
	int ok4 = -1;
	int of;
	yk = strstr(b, "DO"); // DO входит в строку
	if (yk != NULL) {
		*c_do = *c_do + 1;
		if (strlen(b) == 2) {
			if (*c_do > *c_loop) { 
				co = 1; er = 14; // количество DO некорректное
				ok1 = 1;
			}
			else {
				co = 1;	er = 0;
				ok1 = 1;
			}
		}
		else { 
			co = 1;	er = 1; // неправильное написание команды DO
			ok1 = 0;
		}
	}

	if (ok1 == -1) {
		yk = strstr(b, "END"); // END входит в строку
		if (yk != NULL) {
			*c_end = *c_end + 1;
			if (strlen(b) == 3) {
				if (*c_end > *c_loop || *c_end > *c_do) { 
					co = 2; er = 15; // количество END некорректное
					ok2 = 0;
				}
				else {
					co = 2;	er = 0;
					ok2 = 1;
				}
			}
			else {
				co = 2;	er = 2; // неправильное написание команды END
				ok2 = 0;
			}
		}

		if (ok2 == -1 && ok1 == -1) {
			yk = strstr(b, "LOOPx"); // LOOPx в строке 
			if (yk != NULL) {
				*c_loop = * c_loop + 1;
				if (yk - b != 0 || strlen(b) == 5) {
					co = 3;	er = 3; // перед LOOPx неккоректная запись
					ok3 = 0;
				}
				else {
					of = 1;
					for (size_t i = 5; i < strlen(b); i++) {  
						if ((int)b[i] < 48 || (int)b[i] > 57) { // после х стоит число, состоящее не из цифр (проверка по ASCII)
							of = 0;
						}
						if (!of) {
							co = 3;	er = 4; // ошибка в индексе х после LOOP
							ok3 = 0;
						}
						else {
							p1 = atoi(yk + 5); // в p1 будет храниться индекс х
							if (p1 <= INDLIMIT && p1 >= 0) { 
								co = 3;	er = 0; 
								ok3 = 1;
							}
							else {
								co = 3; er = 24; // превышен лимит индексов
								ok3 = 0;
							}
							
						}
					}
				}
			}

			if (ok3 == -1 && ok2 == -1 && ok1 == -1) {
				yk = strstr(b, ":="); // := в строке
				if (yk != NULL) {
					int bsc = strlen(b);
					char *left = malloc(bsc * sizeof(char)); // левая часть выражения xN:=xM+C, т.е xN
					char *right = malloc(bsc * sizeof(char)); // правая часть выражения xN:=xM+C, т.е xM+C
					char *rleft = malloc(bsc * sizeof(char)); // xM
					char *rright = malloc(bsc * sizeof(char)); // C
					strncpy(left, b, yk - b);
					left[yk - b] = '\0';
					strcpy(right, yk + 2); 
					yk = strstr(left, "x"); // x в левой части строки
					if (yk == NULL || yk - left != 0 || strlen(yk) == 1) {
						co = 4;	er = 6; // неправильное написание команды левой части перед :=
						ok4 = 0;
					}
					else {
						of = 1;
						for (size_t i = 1; i < strlen(left); i++) { 
							if ((int)left[i] < 48 || (int)left[i] > 57) { // после х не стоит число, состоящее из цифр (проверка по ASCII)
								of = 0;
							}
						}
						if (!of) {
							co = 4;	er = 7; // ошибка в индексе х перед :=
							ok4 = 0;
						}
						else {
							p1 = atoi(left + 1); // в p1 будет храниться индекc х
							if (p1 > INDLIMIT || p1 < 0) {
								co = 4; er = 24;
								ok4 = 0;
							}
						}
					}
					if(ok4 != 0) {

						yk = strstr(right, "+"); // + в правой части строки
						int plus = 1;
						if (yk == NULL) {
							yk = strstr(right, "-"); // - в правой части строки
							plus = 0;
						}
						if (yk == NULL) {
						co = 4;	er = 8; // не найден +/-
						ok4 = 0;
						}
						else {
							strncpy(rleft, right, yk - right);
							rleft[yk - right] = '\0';
							strcpy(rright, yk + 1);
							yk = strstr(rleft, "x");
							if (yk == NULL || yk - rleft != 0 || strlen(yk) == 1) {
								co = 4;	er = 5; // неправильное написание команды левой части перед +-
								ok4 = 0;
							}
							else {
								of = 1;
								for (size_t i = 1; i < strlen(rleft); i++) //!!!
									if ((int)rleft[i] < 48 || (int)rleft[i] > 57) // после х не стоит число, состоящее из цифр (проверка по ASCII)
										of = 0;
								if (!of) {
									co = 4;	er = 5; // неправильное написание команды левой части перед +-
									ok4 = 0;
								}
								else {
									p2 = atoi(rleft + 1); // в p2 будет храниться индекс хM  
									if (p2 > INDLIMIT || p2 < 0) {
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
											co = 4;	er = 23; // неправильное написание команды првой части после +-
											ok4 = 0;
										}
										else {
											if (plus)
												co = 4;
											else
												co = 5;
											er = 0;
											p3 = atoi(rright); // в p3 будет храниться C
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


// проверка синтаксиса строки файла переменных
int checkvline(char* b, int ns, DblLinkedList * list_init) {
	char * yk;
	Command * com;

	yk = strstr(b, "=");
	int co = 6;
	int er = 0;
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int of;
	if (yk != NULL) {
		int bsc = strlen(b);
		char *left = malloc(bsc * sizeof(char));
		char *right = malloc(bsc * sizeof(char));
		strncpy(left, b, yk - b); // xN, где xN=C
		left[yk - b] = '\0';
		strcpy(right, yk + 1); // С, где xN=C
		yk = strstr(left, "x");
		if (yk == NULL || yk - left != 0 || strlen(left) == 1) {
			co = 6;	er = 10; // неправильное написание х в левой части
		}
		else {
			of = 1;
			for (size_t i = 1; i < strlen(left); i++) {
				if ((int)left[i] < 48 || (int)left[i] > 57) { 
					of = 0;
				}
			}
			if (!of) {
				co = 6;	er = 11; // после х не цифровые символы 
			}
			else {
				p1 = atoi(left + 1); // в p1 будет храниться индекс х в левой части 
				if (p1 > INDLIMIT || p1 < 0) {
					co = 6; er = 24; // индекс превышен
				}
				else {
					of = 1;
					for (size_t i = 1; i < strlen(right); i++)
						if ((int)right[i] < 48 || (int)right[i] > 57) //!!!
							of = 0;
					if (!of) {
						co = 6;	er = 12; // некорректная записать в правой части 
					}
					else {
						p2 = atoi(right); // в p2 будет константа правой части 
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