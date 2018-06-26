#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct _Command {
	int ns;
	char * buffer;
	int command;
	int error;
	size_t p1;
	size_t p2;
	size_t p3;
} Command;
typedef struct _LoopEl {
	int count;
	Node *yk;
} LoopEl;


typedef struct _VarEl {
	int index;
	int value;
} VarEl;


Command * createCommand(int, char *, int, int, size_t, size_t, size_t);
void printCommand(void *, FILE *);
int doCommand(void *, DblLinkedList *, DblLinkedList *, Node **);
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

