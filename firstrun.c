#include "assembler.h"

/*variables declerations*/
static int IC=100,DC=0;
static char line[N];
static char other[N];
static char params[N];
static char label[N];
static char command[N];
static char operation[N];
static char file_name1[N];

/*function returns IC value*/
int get_ic(void)
{
	return IC;
}

/*function returns DC value*/
int get_dc(void)
{
	return DC;
}

/*first_run function recives file name and number from the main , first of all the function opens the file
 if failed the function print error , if succes The function starts reading the file line by line .
 the function Analyzes if the line have label or not , checks if the line is instructional sentence
 or extern sentence or entry sentence , in all cases it calls The appropriate function */
int first_run(char *arr[],int i)
{	
	char file_type1[5] = {".as"} ;
	int e=0,n=0,linenumber=1,flag=0;
	FILE *fd1;
	node_t* temper1;

	IC=100,DC=0;
	
	/*make the correct name for the input file*/
	strcpy(file_name1,arr[i]);
	strncat(file_name1,file_type1,N);

	/*checking if failed to open the inserted file number i*/
	if(!(fd1=fopen(file_name1,"r")))
	{
		printf("error : cannot open file number %d \n",i);
		return -10;
	}
	else
	{
		/*start reading the file , it keeps reading until reaching end of file*/
		while(!feof(fd1))
		{	
			/*saving the line into array for analyzing*/		
			fgets(line,N,fd1);
			/*checking if line length greater than 81 if yes its error and flag = -1*/	
			if(strlen(line)>81)
			{
				printf("error , line %d size greater than 80 \n",linenumber);
				flag = -1;
			}
			else
			{	
				/*if e=3 there is label ,if e=2 label without command , if e=1 no label 
				  otherwise there is error*/					
				e = (sscanf(line, "%[^:]%[:]%[^\n\r]",label,other,command));
				
				/*we have label , checking if there is space between : and the rest of the line */
				if((e==3)&&((strcmp(other,":"))==0)&&((command[0]=='\t')||(command[0]==' ')))
				{
					/*label legal length is 31 , if greater then error and flag = -1*/
					if(strlen(label)>31)
					{
						printf("error : the label lenght at line %d greater than 31\n",linenumber);
						flag = -1;
					}
					else
					{		
							/*in case e=3 we check by scanning command if we have operation with params or not,
							  checking if operation part is instructional sentence or extern\entry sentence
  	 						  in all cases it calls The appropriate function,with the appropriate values */
						n = (sscanf(command,"%s %[^\n\r]",operation,params));
						if(n==2)					
						{
							if((strcmp(operation,".data"))==0||(strcmp(operation,".string"))==0)
							{
								flag = symbol_table(DC,label,"data",linenumber);
								if (flag!=-1)
								{
									flag = data_analysis(operation,params,linenumber, &DC);	
								}
							}
							else if((strcmp(operation,".extern"))==0)
							{
								printf("warning .extern directive after label at line %d\n",linenumber);
								flag = symbol_table(0,params,"external",linenumber);
							}
							else if((strcmp(operation,".entry"))==0)
							{
								printf("warning .entry directive after label at line %d\n",linenumber);		
							}
							else if((strcmp(operation,"stop"))==0||(strcmp(operation,"rts"))==0)			
							{
								printf("error : %s operation with operands at line %d\n",operation,linenumber);
								flag = -1;			
							}					
							else
							{
								flag = symbol_table(IC,label,"code",linenumber);					
								if(flag!=-1)
								{
									flag = code_analysis(operation,params,linenumber, &IC);			
								}
							}			
						}
						/*the case if we have label with operation without params , it checks if its legal
						  instruction or not , and checks if extern\entry sentence ,if its legal instruction
						  or legal entry\extern sentence , it calls the appropriate function,
						  with the appropriate values , or reporting error were founded.*/
						else if(n==1)		
						{	
							/*in this case we have operation with no params , so we have to remove 
							  all chars from params array that were saved from previous analzysis
							   to avoid future probelms with analyzis !*/
							int i = 0;
							while(i<strlen(params))
							{
								params[i]='\0';/*putting end of string char on every index of the array*/
							}
		
							if((strcmp(operation,"stop"))==0||(strcmp(operation,"rts"))==0)
							{
								flag = symbol_table(IC,label,"code",linenumber);
								if(flag!=-1)
								{	
					 				flag = code_analysis(operation,params,linenumber, &IC);	
								}
							}
							else if((strcmp(operation,".extern"))==0||(strcmp(operation,".entry"))==0)
							{
								printf("error : directive sentence with no label after at line %d\n",linenumber);
								flag = -1;								
							}		
							else
							{
								printf("error : illegal instruction at line %d\n",linenumber);
								flag = -1;
							}
						}
					}
				}

				/*no label case , it Does the same job as the case we have label
				  checking if we have legal instruction or extern\entry sentence or data sentence
				  if yes it calls the appropriate functions , if illegal reporting error. */
 			
				else if(e==1)
				{
					int i = 0;
					while((label[i]==' ')&&(label[i]=='\t'))
					{
						i++;
					}
					if(label[i]==';')
					{
						printf("line %d is a comment line\n",linenumber);
					}
					else
					{
						n = (sscanf(label,"%s %[^\n\r]",operation,params));				
						if(n==2)
						{
							if((strcmp(operation,".data"))==0||(strcmp(operation,".string"))==0)
							{
								flag = data_analysis(operation,params,linenumber, &DC);	
							}	
													
							else if((strcmp(operation,".extern"))==0)
							{
								flag = symbol_table(0,params,"external",linenumber);
							}
							else if((strcmp(operation,".entry"))==0)
							{
								/*do in second run*/		
							}	
							else if((strcmp(operation,"stop"))==0||(strcmp(operation,"rts"))==0)
							{
								printf("error : %s operation with operands at line %d\n",operation,linenumber);	
								flag = -1;
							}
							else
							{				
								flag = code_analysis(operation,params,linenumber, &IC);			
							}	
						}			 			
						else if(n==1)
						{	
							int i = 0;
							while(i<strlen(params))
							{
								params[i]='\0';
							}
		
							if((strcmp(operation,"stop"))==0||(strcmp(operation,"rts"))==0)
							{
								flag = symbol_table(IC,label,"code",linenumber);
								if(flag!=-1)
								{						 		
									flag = code_analysis(operation,params,linenumber, &IC);	
								}
							}
							else
							{	
								printf("error : illegal instruction at line %d\n",linenumber);
								flag = -1;
							}
						}
						else if(n==-1)
						{
							printf("line %d is a whitespaces line!!\n",linenumber);
						}
						else
						{
							printf(" wrong syntax at line %d\n",linenumber);
						}			
					}			
				}
				/*empty line*/
				else if(e==-1)
				{
					printf("newline at line %d\n",linenumber);
				}
				/*we have label with ':' after without command after it .*/
				else if(e==2)
				{
					flag = symbol_table(IC,label,"code",linenumber);
				}	
				else
				{	/*wrong instruction syntax*/
					printf(" wrong syntax at line %d or label with no space after ':'\n",linenumber);
					flag = -1;
				}
			}
			linenumber++;			
		}
	}
	
	temper1 = iterator_symbol_init();
	if(flag != -1)
	{
		/*if no errors were founded , we updating the DC address by IC value*/
		while(temper1!=NULL)
		{	
			if((strcmp(temper1->Characterization,"data"))==0)
				temper1->value = temper1->value + get_ic();

			temper1 = temper1->next;
		}
	}

	fclose(fd1);
	return flag;
}
	
	
