#pragma once
#ifndef ILOOP_H
#define ILOOP_H

#include <stdio.h>
#include <stdlib.h>
#define COMLIMIT 100 // максимальная длина команды
int load_prog(FILE *, DblLinkedList *);
int load_var(FILE *, DblLinkedList *);
int checkSyntax(FILE* , DblLinkedList * , DblLinkedList * , DblLinkedList * , DblLinkedList * );
void doProg(FILE *, DblLinkedList * , DblLinkedList * , int);
#endif ILOOP_H

