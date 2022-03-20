#include "assembler.h"

/*commant_t struct pointers declerations*/
static command_t *head = NULL;
static command_t *tail = NULL;
static command_t *iterator = NULL;

/*function to get the head of the command linked list*/
command_t* iterator_init(void)
{
	iterator = head;
	return iterator;
}

/*function to get the tail of the command linked list*/
command_t* iterator_next(void)
{
	if (iterator != NULL)
		iterator = iterator->next;

	return iterator;
}

/*freeing all command linked list nodes to avoid garbage values on the future*/
void free_commands_linked_list_nodes(void)
{
	command_t* tempor1;
	while(head!=NULL)
	{
		tempor1 = head;
		head = head->next;
		free(tempor1);
	}

	head = NULL;
	tail = NULL;
}

/*function adds command_t node to the tail of the command linked list
  the function is static , only on this file we can use it*/
static command_t* alloc_new_command(void)
{
	command_t *temp = NULL ;
	
	temp = malloc(sizeof(command_t));
	
	if (head == NULL) {
		head = temp;
		tail = temp;
	}

	temp->next = NULL;
	return temp;
}

/*after analyzing the instruction on analysis file functions we call add_code_to_command_linked_list
  function with the operation number and params . this function updates the type filed with zero
  in all cases and length filed to one . then its switch case on the operation number to get the right case.
  in every case the function fills the fields with the With the appropriate values according to the assembler
  rules , in some case we call word_analysis_first_param and word_analysis_second_param
  and in some case we call only word_analysis_second_param for filling the rest of fileds of the node
  depends on registers addressing method and the opcode filed after updating it.
  in  case stop or rts we dont need to call the functions etc.   */
int add_code_to_command_linked_list(int opc,char par1[],char par3[], int linenum)
{	
	int flag = 0;
	command_t *current;

	current = alloc_new_command();
	if (current == NULL)
		return -1;

	tail->next = current;
	tail = current;	

	current->type = 0;
	current->word.length = 1;
	switch(opc)
	{	
		/*mov operation case*/
		case 0:		
			
			current->word.opcode = 0;
			current->word.sourcadd = get_reg_add(par1);
			flag=word_analysis_first_param(current->word.sourcadd,current->word.opcode,linenum,par1,current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.destadd = get_reg_add(par3);
			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*cmp operation case*/
		case 1:	

			current->word.opcode = 1;
			current->word.sourcadd = get_reg_add(par1);
			flag = word_analysis_first_param(current->word.sourcadd,current->word.opcode,linenum,par1, current);
			
			if(flag == -1)
			{
				return -1;
			}				

			current->word.destadd = get_reg_add(par3);
			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}				
		
			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*add operation case*/
		case 2:	
				
			current->word.opcode = 2;	
			current->word.sourcadd = get_reg_add(par1);
			flag = word_analysis_first_param(current->word.sourcadd,current->word.opcode,linenum,par1, current);

			if(flag == -1)
			{
				return -1;
			}

			current->word.destadd = get_reg_add(par3);
			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=1;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*sub operation case*/
		case 3:	
			
			current->word.opcode = 2;			
			current->word.sourcadd = get_reg_add(par1);
			flag = word_analysis_first_param(current->word.sourcadd,current->word.opcode,linenum,par1, current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.destadd = get_reg_add(par3);
			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=2;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*lea operation case*/
		case 4:
	
			current->word.opcode = 4;
			current->word.sourcadd = get_reg_add(par1);		
			flag = word_analysis_first_param(current->word.sourcadd,current->word.opcode,linenum,par1, current);
			if(flag == -1)
			{
				return -1;
			}

			current->word.destadd = get_reg_add(par3);
			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*clr operation case*/
		case 5:
			
			current->word.param1_type = -1;
			current->word.opcode = 5;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}
		
			current->word.funct=1;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*not operation case*/
		case 6:
			
			current->word.param1_type = -1;
			current->word.opcode = 5;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=2;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;
		
			break;

		/*inc operation case*/
		case 7:
			
			current->word.param1_type = -1;
			current->word.opcode = 5;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
					
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=3;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*dec operation case*/
		case 8:
			
			current->word.param1_type = -1;
			current->word.opcode = 5;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}
						
			current->word.funct=4;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*jmp operation case*/
		case 9:

			current->word.param1_type = -1;
			current->word.param1_type = -1;
			current->word.opcode = 9;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag = word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}
						
			current->word.funct=1;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*bne operation case*/
		case 10:
			
			current->word.param1_type = -1;
			current->word.opcode = 9;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}
		
			current->word.funct=2;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*jsr operation case*/
		case 11:
			
			current->word.param1_type = -1;
			current->word.opcode = 9;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}
		
			current->word.funct=3;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*red operation case*/
		case 12:
			
			current->word.param1_type = -1;
			current->word.opcode = 12;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}
		
			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*prn operation case*/
		case 13:
			
			current->word.param1_type = -1;
			current->word.opcode = 13;
			current->word.sourcadd=0;
			current->word.sourcreg=0;
			current->word.destadd=get_reg_add(par3);

			flag=word_analysis_second_param(current->word.destadd,current->word.opcode,linenum,par3, current);
			
			if(flag == -1)
			{
				return -1;
			}

			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;

			break;

		/*rts operation case*/
		case 14:

			current->word.opcode = 14;
			current->word.sourcadd=0;
		 	current->word.sourcreg=0;
			current->word.destadd=0;
			current->word.destreg=0;
			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;
			break;

		/*stop operation case*/
		case 15:
		
			current->word.opcode = 15;
			current->word.sourcadd=0;
		 	current->word.sourcreg=0;
			current->word.destadd=0;
			current->word.destreg=0;
			current->word.funct=0;
			current->word.A=1;
			current->word.R=0;
			current->word.E=0;
			break;
	}
	
	return current->word.length;
}

/*this fuction called by data_analysis function when data instruction were found.
  its analysis the value of the integer until ',' found , adding it to the command linked list
  and updating the type filed to one to know that is a data instruction for future use.*/
void add_data_to_command_linked_list(char par[],int counter)
{
	command_t *current;
	char temper[N];	
	int i=0,j=0;
	
	/*copying the legal integer before ',' to a new array for analyzing the value of it
	  counter have the number of integers on the current instruction.*/	
	while(counter>0)
	{			
		while((par[i]!=',')&&(par[i]!='\0'))
		{
			temper[j++]=par[i++];
		}

		current = alloc_new_command();

		if (current == NULL)
		{
			printf("error : issue with malloc ");
			return ;
		}

		tail->next = current;
		tail = current;

		current->type=1;			
		current->data_t.integer = atoi(temper); 
		current->next = NULL;

		/*putting whitespace on all temper array for Future calculations*/
		while(j>=0)
		{
			temper[j]=' ';
			j--;
		}
		
		i++;
		counter--;
		j=0;	
	}
}

/*this fuction called by data_analysis function when data (string) instruction were found.
  its analyzis the value of each char until '"' found , adding it to the command linked list
  and updating the type filed to one to know that is a data instruction for future use.
  in the beggining we running on while loop until we meet the first '"" char
  from this index+1 we start analyzing the value of each char until we meet again the '"' char
  then we updating the fileds with the correct values.*/
void add_string_to_command_linked_list(char par[],int counter)
{
	command_t *current;
	int i = 0;
	while(par[i]!='"')
		i++;

	i++;
	/*while loop runs on the amount of chars we found between the two '""*/
	while(counter>-1)
	{
		current = alloc_new_command();
		if (current == NULL)
		{
			printf("error : issue with malloc ");
			return ; 
		}

		tail->next = current;
		tail = current;

		current->type=1;		
		current->data_t.integer = par[i]; 
		current->next = NULL;
		i++;
		counter--;	

		/*adding another node for (end of string char) to the command linked 
          list right after the string ends.*/
		if(counter == 0)
		{
			counter--;
			current = alloc_new_command();
			if (current == NULL)
			{
				printf("error : issue with malloc ");
				return ; 
			}
			tail->next = current;
			tail = current;

			current->type=1;		
			current->data_t.integer = 0; 
			current->next = NULL;
		}	
	}
}
