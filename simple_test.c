#include "ft_printf.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <limits.h>
#include <float.h>

void print_bit(int n)
{
	int largest = 0;
	int i, temp;
	i = -1;
	temp = n;
	while(temp)
	{
		temp /= 2;
		++i;
	}
	while (i >= 0)
	{
		if (n & 1 << i)
			printf("1");
		else
			printf("0");
		--i;
	}
	printf("\n");
}
			
void test_get_flags()
{
	t_fs f_str = {.flags = 0, .str = "# - 0 + going on\n"};

	get_flags(&f_str);
	print_bit(f_str.flags);
	
	return ;
}

void test_is_conversion()
{
	printf("Is_conversion should return 1 and returns: %d\n", is_conversion('X'));
	printf("Is_conversion should return 0 and returns: %d\n", is_conversion('z'));
}

void test_get_width()
{
	t_fs f_str = {.width = 0, .str = "4901"};
	get_width(&f_str);
	printf("test_get_width expected: 4901, result: %d\n", f_str.width);
	f_str.width = 0;
	f_str.str = "2";
	get_width(&f_str);
	f_str.str = "7";
	get_width(&f_str);
	f_str.str = "1";
	get_width(&f_str);
	printf("test_get_width expected: 7, result: %d\n", f_str.width);
}

void test_get_precision()
{
	int i = 0;

	t_fs f_str = {.width = 0, .str = ".109"};
	get_precision(&f_str);
	printf("Precision expected: 109, result: %d\n", f_str.precision);
	f_str.str = "....";
	f_str.precision = 0;
	get_precision(&f_str);
	printf("Precision expected: 0, result: %d\n", f_str.precision);
	f_str.str = "....734";
	f_str.precision = 0;
	get_precision(&f_str);
	printf("Precision expected: 734, result: %d\n", f_str.precision);
	f_str.str = "....0734";
	f_str.precision = 0;
	get_precision(&f_str);
	printf("Precision expected: 734, result: %d\n", f_str.precision);
	f_str.str = "..5..07..1";
	f_str.precision = 0;
	while(i++ < 3)
		get_precision(&f_str);
	printf("Precision expected: 7, result: %d\n", f_str.precision);
}

void test_get_modifiers()
{
	t_fs f_str = {.modifier = 0};	

	f_str.modifier = 0;
	f_str.str = "L";
	get_modifiers(&f_str);
	print_bit(f_str.modifier);	 
	f_str.modifier = 0;
	f_str.str = "Llll";
	get_modifiers(&f_str);
	print_bit(f_str.modifier);	 
	f_str.modifier = 0;
	f_str.str = "Lllhl";
	get_modifiers(&f_str);
	print_bit(f_str.modifier);	 
	f_str.modifier = 0;
	f_str.str = "Lhh";
	get_modifiers(&f_str);
	print_bit(f_str.modifier);	 
}

void how_convers()
{
	long double ld = 0;
	int 		d = INT_MAX;

	printf("Ld: %d\n", ld);
}

void test_d_i()
{
	ft_printf("helllo %d\n", INT_MAX);
	ft_printf("helllo %d\n", 0);
	ft_printf("helllo %d\n", INT_MIN);
	ft_printf("helllo %+d\n", INT_MAX);
	ft_printf("helllo %+d\n", 0);
	ft_printf("helllo %+d\n", INT_MIN);
	ft_printf("helllo %-d\n", INT_MAX);
	ft_printf("helllo %-d\n", 0);
	ft_printf("helllo %-d\n", INT_MIN);
	ft_printf("helllo %#d\n", INT_MAX);
	ft_printf("helllo %#d\n", 0);
	ft_printf("helllo %#d\n", INT_MIN);
	ft_printf("helllo %0d\n", INT_MAX);
	ft_printf("helllo %0d\n", 0);
	ft_printf("helllo %0d\n", INT_MIN);
	ft_printf("helllo % d\n", INT_MAX);
	ft_printf("helllo % d\n", 0);
	ft_printf("helllo % d\n", INT_MIN);
	ft_printf("helllo |% 4d|\n", 3);
	ft_printf("helllo |%4 d|\n", 0);
	ft_printf("helllo |% 4d|\n", 3);
	ft_printf("helllo |% .4d|\n", 3);
	ft_printf("helllo |%.4 d|\n", 0);
	ft_printf("helllo |% .4d|\n", 3);
	ft_printf("helllo |%0.4d|\n", 3);
	ft_printf("helllo |%.40d|\n", 0);
	ft_printf("helllo |%0.4d|\n", 3);
	ft_printf("helllo |%0.4d|\n", 3);
	ft_printf("helllo |%.40d|\n", 0);
	ft_printf("helllo |%0.4d|\n", 3);
	ft_printf("helllo |%0-4d|\n", 3);
	ft_printf("helllo |%40-d|\n", 0);
	ft_printf("helllo |%-04d|\n", 3);
	ft_printf("helllo |%0-+4d|\n", 3);
	ft_printf("helllo |%4-+d|\n", 0);
	ft_printf("helllo |%+-04d|\n", 3);
	ft_printf("helllo |%0- 4d|\n", 3);
	ft_printf("helllo |%4- d|\n", 0);
	ft_printf("helllo |% -04d|\n", 3);
	ft_printf("helllo |%0- 4d|\n", 3456789);
	ft_printf("helllo |% -04d|\n", -3456789);
	ft_printf("helllo |%0- +4d|\n", 3);
	ft_printf("helllo |% -0+4d|\n", 3);
	ft_printf("helllo |%0 +4d|\n", 3);
	ft_printf("helllo |%0 +4d|\n", 0);
	printf("helllo |%0 +4d|\n", 3);
	printf("helllo |%0 4d|\n", -3);
}
int main(void)
{
//	test_get_flags();
//	test_is_conversion();
//	test_get_width();
//	test_get_precision();
//	test_get_modifiers();	
//	how_convers();
	test_d_i();
	return (0);
}
