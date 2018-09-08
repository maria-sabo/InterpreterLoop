iloop.exe : iloop.o isyntax.o command.o ilist.o iload.o 
	gcc -o iloop iloop.o isyntax.o command.o ilist.o iload.o 
iloop.o: iloop.c 
	gcc -std=c11 -pedantic -Wall -Wextra -c -o iloop.o iloop.c
isyntax.o: isyntax.c
	gcc -std=c11 -pedantic -Wall -Wextra -c -o isyntax.o isyntax.c
command.o: command.c 
	gcc -std=c11 -pedantic -Wall -Wextra -c -o command.o command.c
ilist.o: ilist.c 
	gcc -std=c11 -pedantic -Wall -Wextra -c -o ilist.o ilist.c
iload.o: iload.c 
	gcc -std=c11 -pedantic -Wall -Wextra -c -o iload.o iload.c


