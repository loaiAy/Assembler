#include <ctype.h>
#include "assembler.h"

/*After we identified that is a code type instruction founded on first run function we call this function.
  this function recives array of the operation and array of the registers and save it on 
  oper and par arrays suitability. it scans the register array to check the amount of the registers .
  in case of two registers we check if the second register build from more than one register
  if yes it reports error and return -1 to first run.if no it checks if the operation(opc param) is legal with 2   	   params if no it reports error also if yes we start clearing white spaces from thw registers to get the
    correct name of it to check if its legal register or not , then we call add_code_to_command_linked_list
    in this cases we have length integer that recives back from the called etc function that he holds 
    the length of words that the instruction needs , and updating IC suitability . if the function find 
    any errors it reports it and returning -1.*/
int code_analysis(char oper[],char par[],int linenum, int* pIC)
{
	int length = 0;
	char par1[N],par2[N],par3[N],par4[N],par5[N];
	int i=0,j=0,e=0,c=0,opc=0,f=0;	

	e = (sscanf(par, "%[^,]%[,] %[^\n\r]",par1,par2,par3));	
	opc = get_opcode(oper);
	if(opc==20)
	{
		printf("error :illegal command at line %d\n",linenum);
		return -1;
	}	

	/*case we have 2 params*/
	if(e==3)
	{
		if((strcmp(par2,","))!=0)
		{
			printf("error : syntax error , or too much ',' between the registers at line %d\n",linenum);
			return -1;
		}

		f = (sscanf(par3, "%[^,] %[^\n\r]",par4,par5));
		if(f>1)
		{
			printf("error : too much params\n");
			return -1;
		}
		if((opc!=0)&&(opc!=1)&&(opc!=2)&&(opc!=3)&&(opc!=4))
		{
			printf("error : too much params at line %d\n",linenum);
			return -1;
		}
		/*clearing white spaces from first register*/
		while((par1[i]!= ' ')&&(par1[i]!= '\t'))
		{	
			i++;									
		}
		c=i;
		while(i<strlen(par1))
		{	
			if((par1[i]== ' ')||(par1[i]== '\t'))
			{	
				i++;
			}					
			else
			{
				printf("error in first operand at line %d\n",linenum);
				return -1;	
			}
		}

		par1[c]='\0';
		j=0;

		/*clearing white spaces from second register*/		
		while((par3[j]!= ' ')&&(par3[j]!= '\t'))
		{
			j++;	
		}					
		c=j;					
		while(j<strlen(par3))
		{								
			if((par3[j]!=' ')&&(par3[j]!='\t'))
			{
				printf("error in second operand at line %d\n",linenum);
				return -1;
				j++;
			}
			j++;
		}
		par3[c]='\0';
		
		length = add_code_to_command_linked_list(opc,par1,par3, linenum);	
		if (length == -1)
		{
			printf("error : illegal instruction at line %d\n",linenum);
			return -1;
		}
		*pIC = *pIC + length;
	}
	/*case we have 1 param*/
	else if(e==1)
	{
		if((opc!=5)&&(opc!=6)&&(opc!=7)&&(opc!=8)&&(opc!=9)&&(opc!=10)&&(opc!=11)&&(opc!=12)&&(opc!=13))
		{
			printf("error : few params at line %d\n",linenum);
			return -1;
		}
		length = add_code_to_command_linked_list(opc,NULL,par1, linenum);
		if (length == -1)
		{
			printf("error : illegal instruction at line %d\n",linenum);
			return -1;
		}
		*pIC = *pIC+length;
	}
	/*no params case , checking if the operand is stop or rts*/
	else if ( ((e==-1)) && ( (strcmp(oper,"stop")==0) || (strcmp(oper,"rts")==0)) )
	{
		length = add_code_to_command_linked_list(opc,NULL,NULL, linenum);
		if (length == -1)
		{
			printf("error : illegal instruction at line %d\n",linenum);
			return -1;
		}
		*pIC = *pIC+length;
	}
	/*no params and no stop or rts case or illegal instruction*/
	else
	{
		printf("error : illegal instruction at line %d\n",linenum);
		return -1;
	}	
	return 0;
}

/*After we identified that is a data type instruction founded on first run function we call this function.
  this function recives array of the operation and array of the registers and save it on 
  oper and par arrays suitability.it checks if the operatin is .string\.data and move to the right place.
  then we start clearing white spaces and counting the length and checking if there is a syntax errors.
  then we call add_data_to_command_linked_list function to save the data on the command linked list
  on different nodes . in both cases we have the counter that holds the amount of words that the current data 
  instruction needs . then we updating DC . if there error were found the function reports it and returning -1.*/
int data_analysis(char oper[],char par[],int linenum, int* pDC)
{						
	int i=0,j=0,counter=0;
	if((strcmp(oper,".data"))==0)	
	{	
		/*clearing whitespaces and increasing the counter by 1 when char founded*/	
		while(i<strlen(par))
		{
			if(par[i]=='+'||par[i]=='-')
			{
				i++;
				if(isdigit(par[i]))
				{
					i++;
					while(isdigit(par[i]))
					{
						i++;
					}
					while((par[i]!=',')&&(par[i]!='\0'))
					{
						if((par[i]!=' ')&&(par[i]!='\t'))
						{							
							printf("error : illegal number at place %d in line %d\n",i,linenum);
							i++;
							return -1;
						}
						else
						{
							i++;
						}
					}
					i++;
					counter++;
				}
				else
				{					
					printf("error : illegal syntax at line %d\n",linenum);
					i++;
					return -1;
				}							
			}
			else if(isdigit(par[i]))
			{
				i++;
				while(isdigit(par[i]))
				{
					i++;
				}
				while((par[i]!=',')&&(par[i]!='\0'))
				{
					if((par[i]!=' ')&&(par[i]!='\t'))
					{						
						printf("error : illegal number at place %d in line %d\n",i,linenum);
						i++;
						return -1;
					}
					else
					{
						i++;
					}
				}
				i++;
				counter++;
			}
			else if(par[i]==' '||par[i]=='\t')
			{
				i++;
			}
			else
			{				
				printf("error : illegal syntax at line %d\n",linenum);
				i++;
				return -1;
			}
		}
		i=0;
		add_data_to_command_linked_list(par,counter);
		*pDC=*pDC+counter;
		
	}
	if((strcmp(oper,".string"))==0)	
	{		
		/*clearing whitespaces and increasing the counter by 1 when number founded*/			
		i = strlen(par)-1;			
		if((par[0]!='"'))
		{			
			printf("error : no '' before the string at line %d\n",linenum);
			return -1;
		}
		else if((par[i]=='"'))
		{
			counter = ((strlen(par))-2);
			for(j=1;j<i;j++)
			{
				if((par[j]>31)&&(par[j]<127)&&(par[j]!='\t'))
				{
					j++;
				}
				else
				{				
					printf("error : illegal string at line %d\n",linenum);
					return -1;
				}
			}
		}
		else if(par[i]==' '||par[i]=='\t')
		{
			i--;
			while(par[i]==' '||par[i]=='\t')
			{
				i--;
			}
			if(par[i]=='"')
			{
				counter = i-1;
				for(j=1;j<i;j++)
				{
					if((par[j]>31)&&(par[j]<127)&&(par[j]!='\t'))
					{
						j++;
					}
					else
					{						
						printf("error : illegal string at line %d\n",linenum);
						return -1;
					}
				}	
			}
			else
			{				
				printf("error : wrong string syntax at line %d\n",linenum);
				return -1;
			}
		}
		else
		{ 
			printf("error : illegal string syntax at line %d\n",linenum);
			return -1;
		}
		add_string_to_command_linked_list(par,counter);
		*pDC=*pDC+counter+1;
	}
	return 0;
}
