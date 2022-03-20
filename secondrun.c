#include "assembler.h"

/*variables declerations*/
static char line2[N];
static char other2[N];
static char params2[N];
static char label2[N];
static char command2[N];
static char operation2[N];
static char file_name2[N];
static char output_file_1[N];
static char output_file_2[N];
static char output_file_3[N];

/*function to make the appropriate name for output files
  and for the input files*/
static void make_files_names(char *arr[],int i)
{
	strcpy(output_file_1,arr[i]);
	strcpy(output_file_2,arr[i]);
	strcpy(output_file_3,arr[i]);
	strcpy(file_name2,arr[i]);
	strncat(file_name2,".as",5);
	strncat(output_file_1,".ent",5);
	strncat(output_file_2,".ext",5);
	strncat(output_file_3,".ob",5);
}

/*functions that second run function use*/
int get_ic(void);
int get_dc(void);

/*function to check if a given label is external label or not*/
static int check_if_external_label(char lab[])
{
	node_t* temp;
	temp = iterator_symbol_init();
	while(temp != NULL)
	{
		if((strcmp(temp->symbol,lab)==0))
		{
			if((strcmp(temp->Characterization,"external")==0)) 
				return 1;
		}
		temp = temp->next;
	}
	return -1;
}

/*we call this function after finishing first run function without any syntax errors .
  first of all this function opens the input file again for reading , and checks if there is entry
  label on the file , if yes , the function checks if the label exists on the symbol label linked list
  if yes the function prints it into the correct file that holds entry label names and addresses , 
  if no the function reports error . after finishing this path the function checks if there is no
  errors were found , if there is no errors it continues to printing the objective and external
  output , if there is errors the function repors error and returns -1.*/
int second_run(char *arr[],int i)
{
	FILE *fp1,*fp2,*fp3,*fd1; 
	command_t* temp;
	node_t* temper;
	int flag1=0,flag2,e=0,ff=0,linenumber=1,address=100,word,imm,addr;	
	char label[N];
	
	make_files_names(arr,i);
	
	if(!(fd1=fopen(file_name2,"r")))
	{
		printf("error : cannot open file number = %d \n",i);
		return -1;
	}
	/*reading the first file after rewinding the pointer to the first of file*/
	while(!feof(fd1))
	{			
		fgets(line2,N,fd1);
		e = sscanf(line2, "%[^:]%[:]%[^\n\r]",label2,other2,command2);

		if(e==3)
		{
			sscanf(command2,"%s %[^\n\r]",operation2,params2);
		}
		else
		{
			sscanf(label2,"%s %[^\n\r]",operation2,params2);
		}

		if((params2[0]=='\t')||(params2[0]==' '))
		{
				int j=0,s=0;
				while((params2[j]=='\t')||(params2[j]==' '))
				{
					j++;
				}
				while(params2[j]!='\0')
				{
					label[s++]=params2[j++];
				}
				label[s]='\0';
		}
		else
		{
			strcpy(label,params2);
		}

		/*check if we have .entry statment*/
		if((strcmp(operation2,".entry")==0))
		{			
			temper = iterator_symbol_init();
			if(!(fp1=fopen(output_file_1, "a")))
				printf("can't open or make file for writing\n");

			/*while loop to check if the entry label exist on the symbol linked list*/
			while(temper != NULL)
			{	
				if((strcmp(temper->symbol,label))==0)
				{	
					/*if yes we add the Characterization of the instruction to the
					  Characterization filed on the symbol linked list.*/
					flag1=1;
					if((strcmp(temper->Characterization,"code"))==0)
					{
						strcpy(temper->Characterization,"code,entry");
						fprintf(fp1,"%s %07d\n",label,temper->value);
					}
					else if((strcmp(temper->Characterization,"data"))==0)
					{
						strcpy(temper->Characterization,"data,entry");
						fprintf(fp1,"%s %07d\n",label,temper->value);
					}
				}
				temper = temper->next;
			}
			fclose(fp1);
			if(flag1!=1)
			{
				flag2 = -1;
				printf("error : symbol at line %d not exist on the symboltable on file = %d\n",linenumber,i);
				puts(label);			
			}		
		}	
	}
	
	if(flag2 == -1)
	{
		return -1;
	}
	
	if(!(fp3=fopen(output_file_3,"w")))
	{
		printf("can't open or make file for writing\n");
		return -1;
	}

	fprintf(fp3,"%7d %6d\n",get_ic()-100, get_dc());
	temp = iterator_init();
	while (temp != NULL)
	{ 
		if (temp->type == 0)
		{
			/*printing the first word and its address*/
			fprintf(fp3,"%07d ", address);
			word = (temp->word.opcode << 18) |
				   (temp->word.sourcadd << 16) |
				   (temp->word.sourcreg << 13) |
				   (temp->word.destadd << 11) |
				   (temp->word.destreg << 8) |
				   (temp->word.funct << 3) |
				   (temp->word.A << 2) |
				   (temp->word.R << 1) |
				   (temp->word.E << 0);
			
			fprintf(fp3,"%06x\n", word);
			address++;
			/*checking if there is more words for the instruction*/
			if (temp->word.length > 1)
			{
				/*if yes we print the address and the fileds of the second word*/
				if (temp->word.param1_type == 0)
				{	
					ff = check_if_external_label(temp->word.labelparam1);
					if(ff==1)
					{
						if(!(fp2=fopen(output_file_2, "a")))
						{
							printf("can't open or make file for writing\n");
						}
						fprintf(fp2,"%s %07d\n",temp->word.labelparam1,address);
						temp->word.A1=0;
						temp->word.R1=0;
						temp->word.E1=1;
						fclose(fp2);
					}
		
					addr = (get_symbol_address(temp->word.labelparam1) << 3) |
						   (temp->word.A1 << 2) | 
						   (temp->word.R1 << 1) | 
						   (temp->word.E1 << 0);

					fprintf(fp3,"%07d ", address);
					address++;
					fprintf(fp3,"%06X\n", addr);
				}
				else if (temp->word.param1_type == 1) 
				{
					imm = (temp->word.immParam1 << 3) |
						  (temp->word.A1 << 2) | 
						  (temp->word.R1 << 1) | 
						  (temp->word.E1 << 0);

					fprintf(fp3,"%07d ", address);
					address++;
					fprintf(fp3,"%06X\n",imm);
				}
				/*if yes we print the address and the fileds of the second word in
                  cases if we have only destination register or we in need to three 
				  words for the instruction , in the last case we print the address
				  and the fields of the third word*/
				if (temp->word.param2_type == 0)
				{
					ff = check_if_external_label(temp->word.labelparam2);
					if(ff==1)
					{
						if(!(fp2=fopen(output_file_2,"a")))
						{
							printf("can't open or make file for writing\n");
						}
						fprintf(fp2,"%s %07d\n",temp->word.labelparam2,address);
						temp->word.A2=0;
						temp->word.R2=0;
						temp->word.E2=1;
						fclose(fp2);
					}
					if(temp->word.destadd == 2)
					{	
						temp->word.distance = ((get_symbol_address(temp->word.labelparam2))-(address-1));
						addr = (temp->word.distance << 3) |
							   (temp->word.A2 << 2) | 
							   (temp->word.R2 << 1) | 
							   (temp->word.E2 << 0);
					}
					else
					{
					addr = (get_symbol_address(temp->word.labelparam2) << 3) |
						   (temp->word.A2 << 2) | 
						   (temp->word.R2 << 1) | 
						   (temp->word.E2 << 0);
					}

					fprintf(fp3,"%07d ", address);
					address++;
					fprintf(fp3,"%06X\n", addr);
				} 
				else if (temp->word.param2_type == 1) 
				{
					imm = (temp->word.immParam2 << 3) |
						  (temp->word.A2 << 2) | 
						  (temp->word.R2 << 1) | 
						  (temp->word.E2 << 0);

					fprintf(fp3,"%07d ", address);
					address++;
					fprintf(fp3,"%06X\n", imm);
				}
			}		
		}
		else if (temp->type == 1)
		{
			fprintf(fp3,"%07d %06X\n", address, temp->data_t.integer);
			address++;
		}
		temp = iterator_next();
	}
	/*closing the files and freeing all node on the symbol linked
      list and command linked list .*/
	fclose(fp3);
	return flag2;
}
