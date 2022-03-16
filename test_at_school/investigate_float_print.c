#include <stdio.h>

int main(void)
{
	printf("%f\n", 42);  
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, 42);  
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, 42.0);  
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, (float)42);  
	printf("With hash: %#d and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, (float)42);  
	return (0);
}
