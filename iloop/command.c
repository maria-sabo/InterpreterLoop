#include "stdafx.h"
#include "ilist.h"
#include "command.h"
#define BSE 100 // максимальная длина сообщения об ошибке



void doProg(FILE *f, DblLinkedList * list_command, DblLinkedList * list_init, int comlimit) {

	DblLinkedList * list_varvalue = createDblLinkedList();
	DblLinkedList * list_loop = createDblLinkedList(); 

	Node *tmp = list_init->head; // встаем в начало списка команд для инициализации переменных
	int ok = 1;
	while (tmp && ok) {
		ok = doCommand((Command *)tmp->value, list_loop, list_varvalue, &tmp); // выполняем поочереди команды инициализации переменных
		printCommand(tmp->value, f); // печать команды
		printVarLoop(f, list_loop, list_varvalue); // печать списка list_varvalue и list_loop
		tmp = tmp->next;
	}
	if (!ok) printf("Variable initialisation error\n");

	tmp = list_command->head; // встаем в начало списка команд
	ok = 1;
	int cnt_temp = 0;
	int cnt_all = 0;
	int decision = 0; // решение продолжить ли работу программы
	while ((tmp && ok) && (decision == 0)) {
		ok = doCommand((Command *)tmp->value, list_loop, list_varvalue, &tmp);
		if (ok != -1) {
			printCommand(tmp->value, f);
			printVarLoop(f, list_loop, list_varvalue);
		}
		tmp = tmp->next;
		cnt_temp++;
		if (cnt_temp == comlimit)
		{
			cnt_all = cnt_all + cnt_temp;
			cnt_temp = 0;
			printf("\n%d commands completed. \n0 - continue \n1 - exit \n", cnt_all);
			
			scanf("%d", &decision);
			if (decision == 1)
				printf("The program is interrupted by user \n");
		}
	}
	if (!ok) printf("The program is interrupted\n");
	
	if (findvar(0, list_varvalue)) {
		printf("x0 = %d\n", getvar(0, list_varvalue));
		if (f != NULL) fprintf(f, "x0 = %d\n", getvar(0, list_varvalue));
	}
	else {
		printf("x0 is not initialised\n");
		if (f != NULL) fprintf(f,"x0 is not initialised\n");
	}

	VarEl *buf1;                       // удаляем, освобождая память
	buf1 = (VarEl *) popFront(list_varvalue);
	while (buf1 != NULL) {
		free(buf1);
		buf1 = (VarEl *) popFront(list_varvalue);
	}
	deleteDblLinkedList(&list_varvalue);
	LoopEl *buf2;
	buf2 = (LoopEl *) popFront(list_loop);
	while (buf2 != NULL) {
		free(buf2);
		buf2 = (LoopEl *)popFront(list_loop);
	}
	deleteDblLinkedList(&list_loop);
}

int doCommand(Command * com, DblLinkedList * list_loop, DblLinkedList * list_varvalue, Node** tp) {
	if (!checkloop(list_loop) && com->command != 2) // выход, если последний loop нулевой и не END
		return -1;
	int ok = 1;
	int val;
	switch (com->command) {
	case 1: // DO 
		break;
	case 2: // END
		*tp = getloop(list_loop, tp);
		break;
	case 3: // LOOPx
		if (!findvar(com->p1, list_varvalue)) {
			com->error = 22;
			ok = 0;
		}
		else
			if (!putloop(com->p1, *tp, list_loop, list_varvalue)) {
				com->error = 21;
				ok = 0;
			}
		break;
	case 4: // :=+
		if (!findvar(com->p2, list_varvalue)) {
			com->error = 22;
			ok = 0;
		}
		else {
			val = getvar(com->p2, list_varvalue);
			putvar(com->p1, val + com->p3, list_varvalue);
		}
		break;
	case 5: // :=-
		if (!findvar(com->p2, list_varvalue)) {
			com->error = 22;
			ok = 0;
		}
		else {
			val = getvar(com->p2, list_varvalue);
			putvar(com->p1, val - com->p3, list_varvalue);
		}
		break;
	case 6: // = 
		putvar(com->p1, com->p2, list_varvalue);
		break;

	}
	return ok;
}
// печать команды в соответствии с разбором ошибок
 void printCommand(void * value, FILE *f) {
	char * er = (char *)malloc(BSE * sizeof(char));
	Command * com;
	com = (Command *)value;
	strcpy(er, "\0");
	switch (com->error) {
	case 0:
		strcpy(er, "\0");
		break;
	case 1:
		strcpy(er, "  er: 1 Garbage characters in the use of DO");
		break;
	case 2:
		strcpy(er, "  er: 2 Garbage characters in the use of END");
		break;
	case 3:
		strcpy(er, "  er: 3 Garbage characters in the use of LOOP-X");
		break;
	case 4:
		strcpy(er, "  er: 4 Garbage characters in the index of LOOP x");
		break;
	case 5:
		strcpy(er, "  er: 5 Garbage characters in the left part of +-");
		break;
	case 6:
		strcpy(er, "  er: 6 Garbage characters in the left part of := ");
		break;
	case 7:
		strcpy(er, "  er: 7 Garbage characters in the index of left part");
		break;
	case 8:
		strcpy(er, "  er: 8 Plus or minus are not found");
		break;
	case 9:
		strcpy(er, "  er: 9 There is unrecognised command");
		break;
	case 10:
		strcpy(er, "  er: 10 Garbage characters in the use of х");
		break;
	case 11:
		strcpy(er, "  er: 11 Garbage characters in the index of x");
		break;
	case 12:
		strcpy(er, "  er: 12 Garbage characters in the right part of = ");
		break;
	case 13:
		strcpy(er, "  er: 13 There is unrecognised command");
		break;
	case 14:
		strcpy(er, "  er: 14 Count of DO greater than LOOP");
		break;
	case 15:
		strcpy(er, "  er: 15 Count of END greater than LOOP or DO");
		break;
	case 21:
		strcpy(er, "  er: 21 The x is not available");
		break;
	case 22:
		strcpy(er, "  er: 22 The x is not initialised");
		break;
	case 23:
		strcpy(er, "  er: 23 Garbage characters in the right part of +- \0");
		break;
	case 24:
		strcpy(er, "  er: 24 Limit of index is exceeded \0");
		break;
	}
	printf("%3d  %s %s  \n", com->ns, com->buffer, er);
	if (f != NULL)
		fprintf(f, "%3d  %s %s  \n", com->ns, com->buffer, er);
	free(er);
} 

Command * createCommand(int i, char *b, int c, int e, size_t p1, size_t p2, size_t p3) {
	Command * com = (Command *)malloc(sizeof(Command));
	if (com != NULL) {
		com->ns = i;
		com->buffer = b;
		com->error = e;
		com->command = c;
		com->p1 = p1;
		com->p2 = p2;
		com->p3 = p3;
	}
	return com;
}

Node * getloop(DblLinkedList * list_loop, Node ** tp) {
	LoopEl *le = (LoopEl *)popBack(list_loop);
	if (le != NULL) {
		le->count--;
		Node *yk = le->yk;
		if (le->count < 1) {
			free(le);
			return *tp; // указатель на текущий узел 
		}
		else
			pushBack(list_loop, le);
		return yk; // указатель на узел, с которого надо повторять 
	}
	else
		return *tp; // указатель на текущий узел
}

int checkloop(DblLinkedList * list_loop) {
	Node * t = list_loop->tail;
	if (t == NULL)
		return 1;
	LoopEl *le = (LoopEl *)t->value;
	if (le->count < 1) 
		return 0;
	else 
		return 1;
}

int putloop(int p1, Node *tmp, DblLinkedList * list_loop, DblLinkedList * list_varvalue) {
	if (findvar(p1, list_varvalue)) {
		int val = getvar(p1, list_varvalue);
		LoopEl *le = createLoopEl(val, tmp->next);
		pushBack(list_loop, le);
		return 1;
	}
	else
		return 0;
}

void putvar(int p1, int value, DblLinkedList * list_varvalue) {
	Node * t = list_varvalue->head;
	VarEl * ve;
	while (t) {
		ve = (VarEl *)t->value;
		if (ve->index == p1) {
			ve->value = value;
			break;
		}
		t = t->next;
	}
	if (t == NULL) {
		ve = createVarEl(p1, value);
		pushBack(list_varvalue, (void *)ve);
	}
}

int getvar(int p1, DblLinkedList * list_varvalue) {
	int value = 0;
	Node * t = list_varvalue->head;
	VarEl * ve;
	while (t) {
		ve = (VarEl *)t->value;
		if (ve->index == p1) {
			value = ve->value;
			break;
		}
		t = t->next;
	}
	return value;
}

int findvar(int p1, DblLinkedList * list_varvalue) {
	int value = 0;
	Node * t = list_varvalue->head;
	VarEl * ve;
	while (t) {
		ve = (VarEl *)t->value;
		if (ve->index == p1) {
			value = 1;
			break;
		}
		t = t->next;
	}
	return value;
}

LoopEl * createLoopEl(int i, Node * tmp) {
	LoopEl *le = (LoopEl *)malloc(sizeof(LoopEl));
	if (le != NULL) {
		le->count = i;
		le->yk = tmp;
	}
	return le;
}


void printVarLoop(FILE * f, DblLinkedList * list_loop, DblLinkedList * list_varvalue) {
	Node *tmp;
	tmp = list_varvalue->head;
	printf("                       ");
	if (f != NULL) fprintf(f, "                       ");
	while (tmp) {
		VarEl * ve = (VarEl *)tmp->value;
			printf("x%d=%d  ", (int)ve->index, (int)ve->value);
			if (f != NULL) fprintf(f, "x%d=%d  ", (int)ve->index, (int)ve->value);
		tmp = tmp->next;
	}
	tmp = list_loop->head;
	while (tmp) {
		if (tmp->value != NULL) {
			LoopEl * le = (LoopEl *)tmp->value;
			Node * no = (Node *)le->yk;
			Command * co = (Command *)no->value;
			printf("%d,%d  ", (int)co->ns, (int)le->count);
			if (f != NULL) fprintf(f, "%d,%d  ", (int)co->ns, (int)le->count);
		}
		tmp = tmp->next;
	}
	printf("\n");
	if (f != NULL) fprintf(f, "\n");
}
void printCommandList (DblLinkedList * list, FILE * f) {
	Node *tmp;
	tmp = list->head;
	int i = 0;
	while (tmp) {
		if (tmp->value != NULL) {
			printCommand(tmp->value, f);
		}
		tmp = tmp->next;
		i++;
	}
	printf("\n");
	if (f != NULL) fprintf(f, "\n");
} 
VarEl * createVarEl(int index, int value) {
	VarEl *ve = (VarEl *)malloc(sizeof(VarEl));
	if (ve != NULL) {
		ve->index = index;
		ve->value = value;
	}
	return ve;
}

