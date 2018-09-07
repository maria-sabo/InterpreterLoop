#include "stdafx.h"
#include "ilist.h"

// реализация двусвязного списка взята из интернета

// создание списка
DblLinkedList* createDblLinkedList() {
	DblLinkedList *tmp;
	tmp = (DblLinkedList*)malloc(sizeof(DblLinkedList));
	tmp->size = 0;
	tmp->head = tmp->tail = NULL;
	return tmp;
}

// удаление списка
void deleteDblLinkedList(DblLinkedList **list) {
	Node *tmp = (*list)->head;
	Node *next = NULL;
	while (tmp) {
		next = tmp->next;
	    free(tmp);
		tmp = next;
	}
	free(*list);
	(*list) = NULL;
}

// вставка вперед
void pushFront(DblLinkedList *list, void *data) {
	Node *tmp = (Node*)malloc(sizeof(Node));
	if (tmp == NULL) {
		return;
	}
	tmp->value = data;
	tmp->next = list->head;
	tmp->prev = NULL;
	if (list->head) {
		list->head->prev = tmp;
	}
	list->head = tmp;

	if (list->tail == NULL) {
		list->tail = tmp;
	}
	list->size++;
}

// удаление первого
void* popFront(DblLinkedList *list) {
	Node *prev;
	void *tmp;
	if (list->head == NULL) {
		return NULL;
	}

	prev = list->head;
	list->head = list->head->next;
	if (list->head) {
		list->head->prev = NULL;
	}
	if (prev == list->tail) {
		list->tail = NULL;
	}
	tmp = prev->value;
	free(prev);

	list->size--;
	return tmp;
}

// вставка в конец
void pushBack(DblLinkedList *list, void *value) {
	Node *tmp = (Node*)malloc(sizeof(Node));
	if (tmp == NULL) {
		return;
	}
	tmp->value = value;
	tmp->next = NULL;
	tmp->prev = list->tail;
	if (list->tail) {
		list->tail->next = tmp;
	}
	list->tail = tmp;

	if (list->head == NULL) {
		list->head = tmp;
	}
	list->size++;
}

// уделение из конца
void* popBack(DblLinkedList *list) {
	Node *next;
	void *tmp;
	if (list->tail == NULL) {
		return NULL;
	}

	next = list->tail;
	list->tail = list->tail->prev;
	if (list->tail) {
		list->tail->next = NULL;
	}
	if (next == list->head) {
		list->head = NULL;
	}
	tmp = next->value;
	free(next);

	list->size--;
	return tmp;
}


// указатель на i элемент
Node* getNth(DblLinkedList *list, size_t index) {
	Node *tmp = list->head;
	size_t i = 0;

	while (tmp && i < index) {
		tmp = tmp->next;
		i++;
	}

	return tmp;
}

// печать списка
void printDblLinkedList(DblLinkedList *list) {
	Node *tmp;
	tmp = list->head;
	int i = 0;
	while (tmp) {
		if (tmp->value != NULL) {
			printf("%s \n", (char *)tmp->value);
		}
		tmp = tmp->next;
		i++;
	}
	printf("\n");
}

int checkMemory(void *buffer){
	if (buffer == NULL) {
		printf("Memory allocation error \n");
		return 0;
	}
	else return 1;
}
