#include "ft_printf.h"
#include "../libft/libft.h"
#include <stdio.h>

void print_bit(int n)
{
	while (n > 0)
	{
		if (n & 1)
			printf("1");
		else
			printf("0");
		n = n >> 1;
	}
	printf("\n");
}
			
void test_get_flags()
{
	t_fs f_str = {.flags = 0, .str = "%# - 0 + going on\n"};

	get_flags(&f_str);
	print_bit(f_str.flags);
	
	return ;
}

void test_is_conversion()
{
	printf("Is_conversion should return 1 and returns: %d\n", is_conversion('X'));
	printf("Is_conversion should return 0 and returns: %d\n", is_conversion('z'));
}

int main(void)
{
	test_get_flags();
	test_is_conversion();
	return (0);
}
