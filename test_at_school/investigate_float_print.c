#include <stdio.h>
#include <limits.h>

int main(void)
{
	char c = -1;
	printf("%hhd\n", c);
	int d = c;
	printf("%hhd\n", d);
	double f = d;
	printf("%hhf\n", f);
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, 42);  
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, 42.0);  
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, (float)42);  
	printf("With hash: %#d and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, (float)42);  
	return (0);
}
