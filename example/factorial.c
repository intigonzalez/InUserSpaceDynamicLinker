#include <stdio.h>

extern char Name[100];
extern int arr[4];


char* name = "Maria Maria!!!";
int* Pepe = arr + 2;

int X = 4;

static int
mul(int a, int b)
{
	return a*b;
}

int
fib(int n)
{
	printf("Calculating fib %d!!!\n", n);
	if (n <= 1)
		return 1;
	return fib(n-1) + fib(n-2);
}

int
sayHi(int n)
{
	printf("Hi %s %d!!!\n", Name, Pepe[0]);
	return 1111;
}


int power(int n)
{
	int i;
	int r = 1;
	for (i = 0 ; i<X ; i++ )
		r *= n;
	return r;
}


int
factorial(int n)
{
	return (n<=1)?1:(n*factorial(n-1));
}

int dobleFact(int n)
{
	X = X - 1;
	return mul(factorial(n),2);
}

int magia(int n)
{
	printf("\t\t\tFactorial is %d, power is %d\n", factorial(1), power(2));
	printf("\t\t\tThe factorial is %d, power is %d\n", factorial(3), power(3));
		
	return factorial(7);
}

/*int main()
{
	printf("Sizeof(void*)=%u\n5!=%d\n", sizeof(void*), factorial(5));
	return 0;
}
*/
