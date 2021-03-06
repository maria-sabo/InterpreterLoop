#include "stdafx.h"
#include "ilist.h"
#include "command.h"
#include "iloop.h"
// #define COMLIMIT 100 // максимальная длина команды
 
int main(int argc, char* argv[])
{
	DblLinkedList * list_prog; // загруженные строки программы
	DblLinkedList * list_var; // загруженные строки с переменными
	DblLinkedList * list_command; // список команд разобранной программы 
	DblLinkedList * list_init; // список команд разобранных переменных

    FILE * file_out;
	FILE * file_prog;
	FILE * file_var;
	
	list_prog = createDblLinkedList();
	list_var = createDblLinkedList();

	list_command = createDblLinkedList();
	list_init = createDblLinkedList();
	// вывод справки
	if (argc == 2 && strcmp(argv[1], "help") == 0) {
		printf(
			"\tFor access the commands used, run the program with the key. 'help' \n"
			"\tFor running the program you need to write paths to files. \n"
			"\tUnnecessary COMLIMIT –  positive integer. Default COMLIMIT = 100.  \n"
			"\tExample: iloop.exe program.txt var.txt out.txt [COMLIMIT] (not necessary)\n\n");
		printf("You can use commands such as this: \n\txN := xM +- C;\n\tLOOP xi\n\tDO\n\tsomething...\n\tEND;\n"
			"You can input parametres such as this: x0 = C1, ... xN = CN \n"
			"You can use only non-negative integer \n");
		printf("Enter a key to continue...\n");
		getchar();
		return 0;
	}
	if (argc < 4){
		printf("Author: Sabo Maria \n"
			"\tThis program interpret programs written in the LOOP language.\n"
			"\tFor access the commands used, run the program with the key. 'help' \n"
		    "\tFor running the program you need to write paths to files. \n"
			"\tUnnecessary COMLIMIT –  positive integer. Default COMLIMIT = 100.  \n"
		    "\tExample: iloop.exe program.txt var.txt out.txt [COMLIMIT](not necessary) \n");
		return 0;
	}
	if (strcmp(argv[1], argv[3]) == 0 || strcmp(argv[2], argv[3]) == 0) {
		printf("Input file name and output file name couldn't equal");
		return 0;
	}
	// необязательный пятый аргумент
	int comlimit;
	if (argc == 5) {
		comlimit = atoi(argv[4]);
		if (comlimit <= 0) {
			printf("Wrong COMLIMIT parameter \n");
			return 0;
		}
	}
	else 
		comlimit = COMLIMIT;


	file_out = fopen(argv[3], "w");
    if (file_out == NULL) {
		printf("Output file %s can not be create \n", argv[3]);
		printf("Enter a key to continue...\n");
		getchar();
		return 0;
	} 
	file_prog = fopen(argv[1], "r");
	if (file_prog == NULL ) {
		printf("File %s is not found \n", argv[1]);
		printf("Enter a key to continue...\n");
		getchar();
		return 0;
	}
	else {
		file_var = fopen(argv[2], "r");
		if (file_var == NULL) {
			printf("File %s is not found \n", argv[2]);
			printf("Enter a key to continue...\n");
			getchar();
			return 0;
		}
		else {
			// выполнение программы, если файлы загружены и синтаксический разбор не имеет ошибок
			if (load_prog(file_prog, list_prog) && load_var(file_var, list_var)) {
				if (checkSyntax(file_out, list_prog, list_var, list_command, list_init))
					doProg(file_out, list_command, list_init, comlimit);
			}
			else
				printf("Load program error\n");
			fclose(file_var);
		}
		fclose(file_prog);
	}
	if (file_out != NULL) fclose(file_out);

	deleteDblLinkedList(&list_prog); // списки и так пустые - удаляем сразу
	deleteDblLinkedList(&list_var);

	Command *buf;                       // удаляем, освобождая память
	buf = (Command*) popFront(list_init);
	while (buf != NULL) {
		free(buf->buffer);
		free(buf);
		buf = (Command*)popFront(list_init);
		}
	deleteDblLinkedList(&list_init);

	buf = (Command*) popFront(list_command);
	while (buf != NULL) {
		free(buf->buffer);
		free(buf);
		buf = (Command*)popFront(list_command);
	}
	deleteDblLinkedList(&list_command);
	printf("Program is completed...\n");
	getchar();
}
