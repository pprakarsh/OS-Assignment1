#include <stdio.h>
#include <unistd.h>
#include "addWrapper.h"

typedef union 
{
	float fl;
	int i;
	struct
    {
    	//using concept of bitfields
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;

    } raw;
} union_float;


float add_syscall(float fl1, float fl2)
{
	union_float num1, num2, res_num; num1.fl = fl1; num2.fl = fl2;
	if(fl1 < 0 || fl2 < 0) 	//to check for negative numbers
	{
		return -1;
	}
	else if(fl1 != fl1 || fl2 != fl2)
	{
		return -2;
	}
	else if(num1.i == 0b01111111100000000000000000000000 || num2.i == 0b01111111100000000000000000000000)
	{
		return -3;
	}
	
	
	//swapping numbers if fl1 < fl2 because algorithm expects num1>num2
	if(fl1 < fl2)
	{
		float temp = fl1;
		fl1 = fl2;	
		fl2 = temp;
	}
	
	num1.fl = fl1;
	num2.fl = fl2;

	res_num.i = syscall(434, num1.i, num2.i); //System call
	if(res_num.raw.exponent>=255)
	{
		return -4;
	}
		
	return res_num.fl;
}
