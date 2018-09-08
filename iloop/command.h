#pragma once
#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct _Command {
	int ns; // номер строки 
	char * buffer; // указатель на строку с командой
	int command; // номер команды
	int error; // номер ошибки
	size_t p1; // индекс переменной слева или переменной в LOOP
	size_t p2; // индекс переменной справа или константа 
	size_t p3; // константа
} Command;

typedef struct _LoopEl {
	int count;
	Node *yk; // указатель на узел, который указывает на элемент типа Command, в котором LOOP, с которого надо повторять
} LoopEl;


typedef struct _VarEl {
	int index;
	int value;
} VarEl;


Command * createCommand(int, char *, int, int, size_t, size_t, size_t);
void printCommand(void *, FILE *);
int doCommand(Command *, DblLinkedList *, DblLinkedList *, Node **);
LoopEl * createLoopEl(int, Node *);
void printVarLoop(FILE *, DblLinkedList *, DblLinkedList *);
void printCommandList(DblLinkedList *, FILE *);

Node * getloop(DblLinkedList *, Node **);
int putloop(int p1, Node *tmp, DblLinkedList *, DblLinkedList *);
void putvar(int p1, int value, DblLinkedList *);
int getvar(int p1, DblLinkedList *);
int findvar(int p1, DblLinkedList *);
VarEl * createVarEl(int, int);
int checkloop(DblLinkedList *);

#endif COMMAND_H

