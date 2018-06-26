#include <stdio.h>
#include <stdlib.h>

int load_prog(char *, DblLinkedList *);
int load_var(char *, DblLinkedList *);
int checkSyntax(FILE* , DblLinkedList * , DblLinkedList * , DblLinkedList * , DblLinkedList * );
void doProg(FILE *, DblLinkedList * , DblLinkedList * , int);

