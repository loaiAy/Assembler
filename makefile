assembler: assembler.o analysis.o commandlinkedlist.o firstrun.o getregs.o opcode.o secondrun.o symboltable.o wordanalysis.o
		gcc -o assembler assembler.o analysis.o commandlinkedlist.o firstrun.o getregs.o opcode.o secondrun.o symboltable.o wordanalysis.o

assembler.o: assembler.c 
	gcc -c -ansi -Wall -pedantic assembler.c 

analysis.o: analysis.c 
	gcc -c -ansi -Wall -pedantic analysis.c

commandlinkedlist.o: commandlinkedlist.c 
	gcc -c -ansi -Wall -pedantic commandlinkedlist.c

firstrun.o: firstrun.c 
	gcc -c -ansi -Wall -pedantic firstrun.c

getregs.o: getregs.c 
	gcc -c -ansi -Wall -pedantic getregs.c

opcode.o: opcode.c 
	gcc -c -ansi -Wall -pedantic opcode.c

secondrun.o: secondrun.c 
	gcc -c -ansi -Wall -pedantic secondrun.c

symboltable.o: symboltable.c 
	gcc -c -ansi -Wall -pedantic symboltable.c

wordanalysis.o: wordanalysis.c 
	gcc -c -ansi -Wall -pedantic wordanalysis.c

clean:
	rm -f *.o
