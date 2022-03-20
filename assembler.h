#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef __MYHEADER__
#define __MYHEADER__

#define N 100

/*structs decleration*/
typedef struct node { 
    char symbol[N];
	unsigned int value;
	char Characterization[15];
    struct node *next; 
} node_t;

typedef struct command {
	unsigned int address;
	unsigned int type;/*0=code or 1=data*/
	struct command *next;
	union {
		struct word {
			unsigned int E:1;
			unsigned int R:1;
			unsigned int A:1;
			unsigned int funct:5;
			unsigned int destreg:3;
			unsigned int destadd:2;
			unsigned int sourcreg:3;
			unsigned int sourcadd:2;
			unsigned int opcode:6;
			unsigned int length;
			unsigned int distance:21;
			unsigned int param1_type;/*=0 if label , =1 if imm value , =3 if register*/
			unsigned int param2_type;/*=0 if label , =1 if imm value , =3 if register*/
			unsigned int E1:1;
			unsigned int R1:1;
			unsigned int A1:1;
			unsigned int E2:1;
			unsigned int R2:1;
			unsigned int A2:1;
			unsigned int immParam1:21;
			unsigned int immParam2:21;
			char labelparam1[31];
			char labelparam2[31];
		}word;

		struct data{
			unsigned int type;/*0=string or 1=data*/
			unsigned int integer:24;
		}data_t;
	};
}command_t;

/*functions*/
int first_run(char *arr[],int i);
int second_run(char *arr[],int i);
int get_reg(char pr1[]);
int get_reg_add(char pr1[]);
int assembler(char *arr[]);
int get_opcode(char op[]);
int word_analysis_first_param(int s , int op , int linenum , char par1[], command_t* head);
int word_analysis_second_param(int s , int op , int linenum , char par1[],  command_t* head);
int add_code_to_command_linked_list(int opc,char par1[],char par3[], int linenum);
int symbol_table(int x,char lab[],char sym[],int linenum);
int get_symbol_address(char* symbol);
int code_analysis(char oper[],char par[],int linenum, int*);
int data_analysis(char oper[],char par[],int linenum, int*);
int check_if_assembly_saved_word(char label[]);
void add_data_to_command_linked_list(char par[],int counter);
void add_string_to_command_linked_list(char par[],int counter);
void free_symbol_linked_list_nodes(void);
void free_commands_linked_list_nodes(void);
command_t* iterator_init(void);
command_t* iterator_next(void);
node_t* iterator_symbol_init(void);
node_t* iterator_symbol_next(void);

#endif
