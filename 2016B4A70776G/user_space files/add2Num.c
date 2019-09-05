//The program adds two floating point number through system call.
//The main function calls a wrapper function to add two floating point numbers and 
//the wrapper function does the system call. 
//Here is some info about wrapper function and system call:
	//Wrapper function:- 	add_syscall--> return type: float,     parameters: float, float
	//System call:-			syscall    --> return type: long int,  parameter: int, int

//The algorithm has been designed using the IEEE single precision, floating point representation.
//However the algorithm may not be precise for some test-cases, because
//according to algorithm implemented, the overflow in addition of mantissa is handled by
//dropping the least significant bit (thus loss in precision).

#include <stdio.h>
#include <unistd.h>
#include "addWrapper.h"

int main(int argc, char *argv[])
{
	float num1, num2, res_num;

	printf("Please enter 2 positive floating point numbers separated by space: \n");
	scanf("%f %f", &num1, &num2);

	res_num = add_syscall(num1, num2); //calls to wrapper function

	if(res_num == -1 || res_num == -2 || res_num == -3)
	{
		printf("\nPLEASE ENTER VALID POSITIVE FLOATING POINT NUMBERS\n");
		return 0;
	}
	else if(res_num == -4)
	{
		printf("\nThe result has overflown. Computation cannot be done\n");
		return 0;
	}
		
	printf("\nThe result of Adding %f with %f displayed below:\n", num1, num2);
	printf("Result using system call  = %f\n", res_num);
	printf("Expected Result is        = %f\n", num1 + num2);

	return 0;
}
