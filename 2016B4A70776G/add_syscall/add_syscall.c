#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include "add_syscall.h"


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

SYSCALL_DEFINE2(add_syscall, int, n1, int, n2)
{
	union_float num1, num2, result_num; 
	num1.i = n1;
	num2.i = n2;
	
	//to check if both nums positive
	if(num1.raw.sign != 0 || num2.raw.sign != 0)
	{
		return 1001; //error_status_code
	}

	//when num2 is zero, boundary case, hidden bit here = 0
	//whereas in Normalized form the code expects the hidden bit = 1  
    if(num2.i == 0)
	{
		return num1.i; //(num1+0)=num1=result
	}

	int exp_diff = (num1.raw.exponent - num2.raw.exponent);


	//FOR DEBUGGING PURPOSE:
		//printf("IEEE 754 representation of %f is : \n", num1.fl);
		//IEEE_rep_print(num1);
		//printf("IEEE 754 representation of %f is : \n", num2.fl);
		//IEEE_rep_print(num2);
		//printf("Exponent difference: %d\n", exp_diff);


	//when exp of both nums (IEEE 754 single precision standard) not equal
	//then make exp equal and correspondingly change mantissa 
	if(exp_diff != 0)
	{	
		//hidden bit is 1 (in normalised form). So, shifting by 1 and
		//putting 1 at msb of mantissa
		num2.raw.mantissa = num2.raw.mantissa >> 1;	
		num2.raw.mantissa = num2.raw.mantissa | 0b10000000000000000000000;


		if(exp_diff != 1)//to check if further shifting required
		{
			num2.raw.mantissa = num2.raw.mantissa >> (exp_diff-1);	
		}
		num2.raw.exponent = num1.raw.exponent; //exponents equal for both nums
	}

	result_num.raw.mantissa = (num1.raw.mantissa)+(num2.raw.mantissa); //adding Mantissa

	//to check for overflow/boundary conditions when adding mantissa
	unsigned int x = num1.raw.mantissa; unsigned int y = num2.raw.mantissa; //converts 23 bits to 32 bits (padding with zeroes)
	int carry; //stores info about overflow
	if(x+y >= 8388608) //8388608 is 2^23 denoting overflow from 23 bits 
	{
		carry = 1;
	}
	else
	{
		carry = 0;
	}

	result_num.raw.sign = 0; //sign bit definately zero
	result_num.raw.exponent = num1.raw.exponent; //initializing it to exp of num1
	
	//preparing result_num when exponent of both num1 and num2 equal
	if(exp_diff == 0)
	{
		//Normalizing result_num according to overflow condition during mantissa addition
		if(carry == 1)
		{
			result_num.raw.mantissa = result_num.raw.mantissa>>1;
			result_num.raw.mantissa = result_num.raw.mantissa | 0b10000000000000000000000; 		
			result_num.raw.exponent += 1;
		}
		else
		{
			result_num.raw.mantissa = result_num.raw.mantissa>>1;
			result_num.raw.exponent += 1;
		}
	}
	else
	{
		if(carry == 1)
		{
			result_num.raw.mantissa = result_num.raw.mantissa>>1;
			result_num.raw.exponent += 1;
		}
		else
		{
			//already normalized in this case;
		}
	}
	return result_num.i;
}

