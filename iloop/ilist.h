#include <stdio.h>
#include <malloc.h>

// узел
typedef struct _Node {
	void *value;
	struct _Node *next;
	struct _Node *prev;
} Node;

// список
typedef struct _DblLinkedList {
	size_t size;
	Node *head;
	Node *tail;
} DblLinkedList;

DblLinkedList* createDblLinkedList();
void deleteDblLinkedList(DblLinkedList **list);
void pushFront(DblLinkedList *list, void *data);
void* popFront(DblLinkedList *list);
void pushBack(DblLinkedList *list, void *value);
void* popBack(DblLinkedList *list);
Node* getNth(DblLinkedList *list, size_t index);
void printDblLinkedList(DblLinkedList *list, void(*fun)(void*));
void printInt(void *value);
void printString(void *value);
