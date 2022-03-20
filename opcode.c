#include <stdio.h>
#include <string.h>

/*opcode assembler table , it recives the operaion and return if it is a legal assembly operatio*/
int get_opcode(char op[])
{
	if(strcmp(op,"mov")==0)
		return 0;
	if(strcmp(op,"cmp")==0)
		return 1;
	if(strcmp(op,"add")==0)
		return 2;
	if(strcmp(op,"sub")==0)
		return 3;
	if(strcmp(op,"lea")==0)
		return 4;
	if(strcmp(op,"clr")==0)
		return 5;
	if(strcmp(op,"not")==0)
		return 6;
	if(strcmp(op,"inc")==0)
		return 7;
	if(strcmp(op,"dec")==0)
		return 8;
	if(strcmp(op,"jmp")==0)
		return 9;
	if(strcmp(op,"bne")==0)
		return 10;
	if(strcmp(op,"jsr")==0)
		return 11;
	if(strcmp(op,"red")==0)
		return 12;
	if(strcmp(op,"prn")==0)
		return 13;
	if(strcmp(op,"rts")==0)
		return 14;
	if(strcmp(op,"stop")==0)
		return 15;
	return 20;
}
