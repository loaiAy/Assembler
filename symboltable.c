#include <ctype.h>
#include "assembler.h"

static node_t *head = NULL;
static node_t *current = NULL;
static node_t *temp = NULL;
static node_t *iterator1 = NULL;
static char saved_word[24][5] =	{"r0","r1","r2","r3","r4","r5","r6","r7","mov",
									"cmp","add","sub","lea","clr","not","inc",
									"dec","jmp","bne","jsr","red","prn","rts","stop"};
	
/*functions this file use*/
int check_sym(node_t *ptr,char lab[]);
int get_symbol_address(char* symbol);

/*function to get the head of the symbol linked list*/
node_t* iterator_symbol_init(void)
{
	iterator1 = head;
	return iterator1;
}

/*function to get the last node on the symbol linked list*/
node_t* iterator_symbol_next(void)
{
	if (iterator1 != NULL)
		iterator1 = iterator1->next;

	return iterator1;
}

void free_symbol_linked_list_nodes(void)
{
	node_t* tempor1;
	while(head!=NULL)
	{
		tempor1 = head;
		head = head->next;
		free(tempor1);
	}

	head = NULL;
}

/*function that checks if label already exist on the symbol linked list*/
int check_sym(node_t *ptr,char lab[])
{
	if(ptr==NULL)
		return 0;

	while(ptr->next!=NULL)
	{
		if(strcmp(ptr->symbol,lab)==0)
		{
			return -1;
		}
		ptr = ptr->next;	
	}
	if(strcmp(ptr->symbol,lab)==0)
	{
		return -1;
	}
	return 0;
}

/*function checks if the label name is a aseembly saved word*/
int check_if_assembly_saved_word(char label[])
{
	int i = 0;	
	while(i<24)
	{
		if(strcmp(label,saved_word[i])==0)
		{
			return -1;
		}
		i++;
	}
	return 0;
}

/*x = DC or IC , lab = label name , sym = data or code or eexternal
  the function calls function that check if the label already exist or if the label name is a assembly saved word.
  if no the function checks if the label name is legal , if no it reports error and returns -1.
  if legal the function adds a node to the symbol linked list for the label with its
  name and address and Characterization  */
int symbol_table(int x,char lab[],char sym[],int linenum)
{
	int i=1,w=0,j=0,s=0;
	char label[30];
	if((lab[0]=='\t')||(lab[0]==' '))
	{
		while((lab[j]=='\t')||(lab[j]==' '))
		{
			j++;
		}
		while(lab[j]!='\0')
		{
			label[s++]=lab[j++];
		}
		label[s]='\0';
	}
	else
	{
		strcpy(label,lab);
	}
	j=0;	
	w = check_if_assembly_saved_word(label);
	if(w==-1)
	{
		printf("error : label with saved assembler word at line %d\n",linenum);		
		return -1;
	}
	j = check_sym(head,label);
	if((strcmp(label,".extern")==0)||(strcmp(label,".entry")==0))
	{
		if(j==-1)
		{
			printf("warning : label name already exist at line %d\n",linenum);
			return 0;
		}
	}
	else
	{
		if(j==-1)
		{
			printf("error : label name already exist at line %d\n",linenum);
			return -1;
		}
	}
	if((label[0]<'A')||((label[0]>'Z')&&(label[0]<'a'))||(label[0]>'z'))
	{
		printf(" error : label don't start with letter at line %d\n",linenum);
		return -1;
	}

	for(i=1;i<strlen(label);i++)
	{
		if((!isdigit(label[i]))&&((label[i]<'A')||((label[i]>'Z')&&(label[i]<'a'))||(label[i]>'z')))
		{
			printf(" error : illegal label name at line %d\n",linenum);
			return -1;
		}
	}
	current = (node_t *)malloc(sizeof(node_t));
	if(head==NULL)
	{
		/*legal label*/
		head = current;
		strcpy(head->symbol , label);
		strcpy(head->Characterization , sym);
		head->value = x;
		head->next	= NULL;
	}
	else
	{
		temp = head ;
		while(temp->next!= NULL)
		{
			temp = temp->next;
		}
		temp->next = current ;			
		strcpy(current->symbol , label);
		strcpy(current->Characterization , sym);
		current->value = x;
		current->next = NULL;
	}
	return 0;
}

/*this function returns the address of the label on file. */
int get_symbol_address(char* symbol)
{
	temp = head;

	while (temp != NULL)
	{
		if((strcmp(temp->symbol,symbol)) == 0)
			return temp->value;
		temp = temp->next;
	}
	return 0;
} 

