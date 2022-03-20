#include "assembler.h"

/*the main checks if there is files have been inserted into the command line argument , if yes it calls
first run function with the file name and number , if there is no errors were found on the file it calls second run  function , if no files have been inserted it prints error!!*/

int main(int argc,char *argv[])
{
	int flag1=0,flag2=0,t=1;
	
	if(argc==1)
	{
		printf("\n error : no input files have been inserted at the command line arguments !! \n");
		exit(0);
	}

	/*while loop calling first_run function and second_run function until there is no more input files*/
	while(t<argc)
	{	
		free_symbol_linked_list_nodes();
		free_commands_linked_list_nodes();
		
		flag1 = first_run(argv,t);
		
		/*if flag1 = -1 there is error were found on file number 't'*/
		if(flag1==-1)
		{
			printf("\nfailed : there is syntax errors in input file number %d\n",t);
			puts("----------------------------------------------------------------------------");
		}
		else if(flag1==0)
		{	
			/*if flag2 = -1 , errors were founded on second run on file number t*/
			flag2 = second_run(argv,t);
			if(flag2==-1)
			{
				printf("fail : there is syntax errors in input file number %d\n",t);
			}
			else
			{
				printf("\nsucces : The output files creation was successful for input file number %d\n\n",t);
				puts("----------------------------------------------------------------------------");
			}	
		}
		t++;
	}	
	return 0;
}

