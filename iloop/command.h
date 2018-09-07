#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#pragma once
#ifndef COMMAND_H
#define COMMAND_H

typedef struct _Command {
	int ns; // ����� ������ 
	char * buffer; // ��������� �� ������ � ��������
	int command; // ����� �������
	int error; // ����� ������
	size_t p1; // ������ ���������� ����� ��� ���������� � LOOP
	size_t p2; // ������ ���������� ������ ��� ��������� 
	size_t p3; // ���������
} Command;

typedef struct _LoopEl {
	int count;
	Node *yk; // ��������� �� ����, ������� ��������� �� ������� ���� Command, � ������� LOOP, � �������� ���� ���������
} LoopEl;


typedef struct _VarEl {
	int index;
	int value;
} VarEl;

// #endif COMMAND_H

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

