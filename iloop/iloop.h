#include <stdio.h>
#include <stdlib.h>

int load_prog(FILE *, DblLinkedList *);
int load_var(FILE *, DblLinkedList *);
int checkSyntax(FILE* , DblLinkedList * , DblLinkedList * , DblLinkedList * , DblLinkedList * );
void doProg(FILE *, DblLinkedList * , DblLinkedList * , int);

