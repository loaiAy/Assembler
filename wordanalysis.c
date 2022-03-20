#include "assembler.h"

/*function recives first register and his addressing method and his opcode . the function checks
  if the addressing type legal to the operation addressing types , if yes its keep filling the
  word fields . this function recognize how much words this command need , in each case that there is
  a ndeed to other word , the function fills the second word fields with the right values
  and in some cases the function recognize there is a need to third word , as well 
  the function fills the fields of the whird word with the right values. if the function found no 
  matching between the opcode of the operation and the addressing type it reports error and return -1;*/
int word_analysis_first_param(int s , int op , int linenum , char par1[], command_t* head)
{
	int ff=0;
	char par[N];
	
	if((op==0)||(op==1)||(op==2))	
	{
		if(s == 2)
		{
			printf("error : wrong source param at line %d\n",linenum);						
			return -1;
		} 
		else if(s == 1)
		{
			head->word.R1 = 1;
			head->word.param1_type = 0;			
			head->word.sourcreg = 0 ;			
			strcpy(head->word.labelparam1, par1);
			head->word.length+=1;
		} 
		else if (s == 0)
		{
			head->word.A1 = 1;
			head->word.param1_type = 1;
			head->word.length+=1;			
			head->word.sourcreg = 0 ;			
			sscanf(par1, "#%s",par);
			head->word.immParam1 = atoi(par);
		} 
		else if (s == 3)
		{
			ff = get_reg(par1) ;
			if((ff<0)||(ff>7))
			{
				return -1;
			}
			head->word.param1_type = 3;
			head->word.sourcreg = ff ;
			head->word.length+=0;	
		}
	}
	else if(op==4)
	{
		if(s!=1)
		{
			printf("error : wrong source param at line %d\n",linenum);
			return -1;
		}
		head->word.R1 = 1;
		head->word.param1_type = 0;
		head->word.length+=1; 
		head->word.sourcreg = 0 ;			
		strcpy(head->word.labelparam1, par1);
	}
	return head->word.length;
}

/*function recives second register and his addressing method and his opcode . the function checks
  if the addressing type legal to the operation addressing type , if yes its keep filling the
  word fields . this function recognize how much words this command need , in each case that there is
  a ndeed to other word , the function fills the second word fields with the right values
  and in some cases the function recognize there is a need to third word , as well 
  the function fills the fields of the whird word with the right values. if the function found no 
  matching between the opcode of the operation and the addressing type it reports error and return -1;*/
int word_analysis_second_param(int s , int op , int linenum , char par2[], command_t* head)
{
	char par[N];
	int ff=0;	
	if((op==0)||(op==2)||(op==4))	
	{
		if((s == 2)||(s == 0))
		{
			printf("error : wrong destantion param at line %d\n",linenum);						
			return -1;
		} 
		else if(s == 1)
		{	
			head->word.R2 = 1;
			head->word.length+=1;		
			head->word.destreg = 0 ;			
			strcpy(head->word.labelparam2, par2);
			head->word.param2_type = 0;
		} 
		else if (s == 3)
		{
			ff = get_reg(par2) ;
			if((ff<0)||(ff>7))
			{
				return -1;
			}
			head->word.destreg = ff;
			head->word.length+=0;
			head->word.param2_type = 3;		
		}
	}
	else if((op==1)||(op==13))
	{
		if(s==2)
		{
			printf("error : wrong destination param at line %d\n",linenum);
			return -1;
		} 
		else if(s == 1)
		{	
			head->word.R2 = 1;
			head->word.length+=1;		
			head->word.destreg = 0 ;			
			strcpy(head->word.labelparam2, par2);
			head->word.param2_type = 0;
		} 
		else if (s == 0)
		{
			head->word.A2 = 1;
			head->word.length+=1;
			head->word.destreg = 0 ;			
			sscanf(par2, "#%s",par);
			head->word.immParam2 = atoi(par);
			head->word.param2_type = 1;
		} 
		else if (s == 3)
		{
			ff = get_reg(par2) ;
			if((ff<0)||(ff>7))
			{
				return -1;
			}
			head->word.destreg = ff;
			head->word.param2_type = 3;
			head->word.length+=0;	
		}
	}
	else if((op==5)||(op==12))
	{
		if(s == 1)
		{	
			head->word.R2 = 1;
			head->word.length+=1;		
			head->word.destreg = 0 ;			
			strcpy(head->word.labelparam2, par2);
			head->word.param2_type = 0;
		} 
		else if(s == 3)
		{
			ff = get_reg(par2) ;
			if((ff<0)||(ff>7))
			{
				return -1;
			}
			head->word.destreg = ff;
			head->word.param2_type = 3;
			head->word.length+=0; 	
		}
		else
		{
			printf("error : wrong destination param at line %d\n",linenum);
			return -1;
		} 
	}
	else if(op == 9)
	{
		if(s == 1)
		{	
			head->word.R2 = 1;
			head->word.length+=1;		
			head->word.destreg = 0 ;			
			strcpy(head->word.labelparam2, par2);
			head->word.param2_type = 0;
		} 
		else if(s == 2)
		{
			head->word.A2 = 1;
			head->word.length+=1;
			sscanf(par2 ,"&%s",par);
			strcpy(head->word.labelparam2, par);
			head->word.param2_type = 0;
		}
		else
		{
			printf("error : wrong destination param at line %d\n",linenum);
			return -1;
		}
	}
	return head->word.length;
}

