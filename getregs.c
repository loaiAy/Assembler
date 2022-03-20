#include <stdio.h>
#include <string.h>

/*function recives register and returns if it is legal assembly register*/
int get_reg(char paar1[])
{
	if((strcmp(paar1,"r0"))==0)
	{
			return 0;
	}
	else if((strcmp(paar1,"r1"))==0)
	{	
		return 1 ;
	}
	else if((strcmp(paar1,"r2"))==0)
	{
		return 2 ;
	}
	else if((strcmp(paar1,"r3"))==0)
	{
		return 3 ;
	}
	else if((strcmp(paar1,"r4"))==0)
	{
		return 4 ;
	}
	else if((strcmp(paar1,"r5"))==0)
	{
		return 5 ;
	}
	else if((strcmp(paar1,"r6"))==0)
	{
		return 6 ;
	}
	else if((strcmp(paar1,"r7"))==0)
	{
		return 7 ;
	}
	else
	{
		return -1 ;
	}	

}

/*function recives register and returns 
what is the register addrresing method*/
int get_reg_add(char pr1[])
{	
		
	if(pr1[0]=='#')
	{
		return 0;
	}
	else if(pr1[0]=='&')
	{
		return 2;
	}
	else if((get_reg(pr1))!=-1)
	{
		return 3;
	}
	return 1; 
}

