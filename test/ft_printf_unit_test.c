#include "get_next_line.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <string.h>
//#include "unity.h"
//#include "unity_internals.h"
#include "ft_printf.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <limits.h>
#include <time.h>

/*	%[$][flags][width][.precision][length modifier]conversion
 *
 */
void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void	write_to_file(char **inputs, char *res, char *res2, int fd, int fd2)
{
	int len;
	int ret;
	len = strlen(res);
	ret = write(fd, res, len);
	len = strlen(res2);
	write(fd2, res2, len);
}

void	dump_it(char **inputs, char *res, char *res2, char *test_name)
{
	int fd, fd2, fd3, i = 0;
	char *expected, *yours, *input;
	fd = open("results/expected.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
	fd2 = open("results/yours.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd < 0 || fd2 < 0)
	{
		printf("Open failed in dump_it\n");
		exit (-1);
	}
	write_to_file(inputs, res, res2, fd, fd2);
	lseek(fd, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);
	fd3 = open("results/errors.txt", O_RDWR | O_APPEND | O_CREAT, 00644);
	if (fd3 < 0)
	{
		printf("Open failed in dump_it\n");
		exit (-1);
	}
	while(get_next_line(fd, &expected) == 1 && get_next_line(fd2, &yours) == 1)
	{
		if(strcmp(expected, yours) != 0)	
		{
			write(fd3, test_name, strlen(test_name));
			write(fd3, "\n", 1);
			write(fd3, "Input   : ", 10);
			write(fd3, inputs[i], ft_strlen(inputs[i])); 
			write(fd3, "Expected: ", 10);
			write(fd3, expected, ft_strlen(expected)); 
			write(fd3, "\nYours   : ", 11);
			write(fd3, yours, ft_strlen(yours)); 
			write(fd3, "\n", 1);
		}
		++i;
	}
	close(fd);
	close(fd2);
	close(fd3);
}

void test_hash_flag(void)
{	
	int buf_size = 300;
	char *test_name = "test_hash_flag";
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//TODO Weird behaviour with the next printf %#f prints just zero or -nan
	inputs[i] = "Argument is 42 or 42.0: %#o and %#x and %#X and %#f and %#.0f\n";
	printf(inputs[i++], 42, 42, 42, 42.0, 42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	i = 0;
	inputs[i] = "Argument is 42 or 42.0: %#o and %#x and %#X and %#f and %#.0f\n";
	printf(inputs[i++], 42, 42, 42, 42.0, 42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_zero_flag(void)
{	
	int buf_size = 300;
	char *test_name = "test_zero_flag";
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//The value is zero padded, - flag overrides 0 flag, with precision for diouxX 0 flag is ignored, for others undefined
	inputs[i] = "With zero flag all arguments are 42: %04d, %04i, %04o, %04u, %04x, %04X, %04f\nWith precision: %04.2d, %04.2i, %04.2o, %04.2u, %04.2x, %04.2X, %04.2f\nWith - flag   : %0-4d, %-04i, %0-4o, %-04u, %0-4x, %-04X, %0-4f\n";
	printf(inputs[i++], 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	printf(inputs[i++], 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42); 
	

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_minus_flag(void)
{	
	char *test_name = "test_minus_flag";
	char res[200];
	char res2[200];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//- overriding 0 is tested in the zero test
	inputs[i] = "Argument is 42 or 42.0: |%-4d|, |%-4i|, |%-4o|, |%-4u|, |%-4x|, |%-4X|, |%-8.1f|\n";
	printf(inputs[i++], 42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	i = 0;
	inputs[i] = "Argument is 42 or 42.0: |%-4d|, |%-4i|, |%-4o|, |%-4u|, |%-4x|, |%-4X|, |%-8.1f|\n";
	printf(inputs[i++], 42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);

	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_space_flag(void)
{	
	char *test_name = "test_space_flag";
	char res[200];
	char res2[200];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//Does nothing with ouxX because those are unsigned conversions not signed!
	inputs[i] = "Argument is +-42 or -+42.0: |% d|, |% d|, |% i|, |% o|, |% u|, |% x|, |% X|, |% f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i] = "Argument is +-42 or -+42.0: |% d|, |% d|, |% i|, |% o|, |% u|, |% x|, |% X|, |% f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);

	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_plus_flag(void)
{	
	char *test_name = "test_plus_flag";
	int buf_size = 300;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//Does nothing with ouxX because those are unsigned conversions not signed!
	inputs[i] = "Arguments +-42 or +-42.0: |%+d|, |%+d|, |%+i|, |%+o|, |%+u|, |%+x|, |%+X|, |%+f|, |%+f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	i = 0;

	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_min_width(void)
{	
	char *test_name = "test_min_width";
	int buf_size = 300;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i] = "Argument is +-42 or +- 42.0: |%6d|, |%6d|, |%6i|, |%6o|, |%6u|, |%6x|, |%6X|, |%8.2f|, |%8.2f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	inputs[i] = "Argument is +-42 or +- 42.0: |%0d|, |%1d|, |%1i|, |%1o|, |%1u|, |%1x|, |%1X|, |%1f|, |%0f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_precision(void)
{	
	int buf_size = 300;
	char *test_name = "test_precision";
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i] = "inputs: +-42 or +-42.0: |%.4d|, |%.4d|, |%.4i|, |%.4o|, |%.4u|, |%.4x|, |%.4X|, |%.4f|, |%.4f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	inputs[i] = "input for all is 'Wisdom': |%.s|, |%.3s|, |%.10s|\n";
	printf(inputs[i++] , "Wisdom", "Wisdom", "Wisdom"); 
	inputs[i] = "Arguments are +-42 or +-42.0: |%.d|, |%.d|, |%.i|, |%.o|, |%.u|, |%.x|, |%.X|, |%.f|, |%.f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	inputs[i] = "inputs: +-42 or +-42.0: |%.4d|, |%.4d|, |%.4i|, |%.4o|, |%.4u|, |%.4x|, |%.4X|, |%.4f|, |%.4f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	inputs[i] = "input for all is 'Wisdom': |%.s|, |%.3s|, |%.10s|\n";
	printf(inputs[i++] , "Wisdom", "Wisdom", "Wisdom"); 
	inputs[i] = "Arguments are +-42 or +-42.0: |%.d|, |%.d|, |%.i|, |%.o|, |%.u|, |%.x|, |%.X|, |%.f|, |%.f|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_length_mod_hh(void)
{	
	char *test_name = "test_length_mod_hh";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	int p[2], p2[2], stdout_bk;
	char *inputs[6];
	int i = 0;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//When integer is given as argument, output is the value which corresponds to conversion. So hh does nothing.
	//With overflow and underflow, trust casting, it causes UB. With overflows then output is argument mod 128, sometimes in decimal form.
	inputs[i] = "Argument for dioucX: +-42: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	int c = -1;
	//with negative value, just the value gets printed. 
	//1111 1111 = 255 and -1.
	//Why float is zero: varg promotes signed char to int, when bit value of -1 is casted to zero, result is such miniscule that it gets rounded to 0. Or float just reads crab memory which is usually zero?
	inputs[i] = "Argument value for all: -1: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], c, c, c, c, c, c, c, c, c); 
	unsigned char ch = -1;
	//ch is now 255, because its just 11...1 converted to unsigned char which is UCHAR_MAX
	//float prints just zeroes, assumably undefined behavior.
	inputs[i] = "Argument is -1 casted to unsigned :D: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch, ch); 
	ch = 0;
	inputs[i] = "Argument is 0: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch, ch); 
	int max = INT_MAX;
	inputs[i] = "Argument is INT_MAX: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], max, max, max, max, max, max, max, max); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i] = "Argument for dioucX: +-42: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	c = -1;
	inputs[i] = "Argument value for all: -1: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], c, c, c, c, c, c, c, c, c); 
	ch = -1;
	inputs[i] = "Argument is -1 casted to unsigned :D: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch, ch); 
	ch = 0;
	inputs[i] = "Argument is 0: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch, ch); 
	inputs[i] = "Argument is INT_MAX: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|\n";
	printf(inputs[i++], max, max, max, max, max, max, max, max); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}
void test_length_mod_h(void)
{	
	char *test_name = "test_length_mod_h";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0, x = 42;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i] = "Argument is +-42: |%hd|, |%hd|, |%hi|, |%ho|, |%hu|, |%hx|, |%hX|\n";
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	signed char c = -1;
	inputs[i] = "Argument is -1: |%hd|, |%hd|, |%hi|, |%ho|, |%hu|, |%hx|, |%hX|\n";
	printf(inputs[i++], c, c, c, c, c, c, c, c); 
	unsigned char ch = c;
	inputs[i] = "Argument is -1 casted to unsigned i.e 255: |%hd|, |%hd|, |%hi|, |%ho|, |%hu|, |%hx|, |%hX|\n";
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch); 
	ch = 0;
	inputs[i] = "Argument is 0: |%hd|, |%hd|, |%hi|, |%ho|, |%hu|, |%hx|, |%hX|\n";
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	i = 0;

	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	c = -1;
	printf(inputs[i++], c, c, c, c, c, c, c, c); 
	ch = c;
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch); 
	ch = 0;
	printf(inputs[i++], ch, ch, ch, ch, ch, ch, ch, ch); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_length_mod_l(void)
{	
	char *test_name = "test_length_mod_l";
	int buf_size = 700;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[6];
	int i = 0, x;
	long	int lx;
	unsigned long int ulx;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i++] = "Argument is +-42: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is 0: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is LONG_MAX: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is LONG_MIN: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	i = 0;
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	x = 0;
	printf(inputs[i++], x, x, x, x, x, x, x); 
	lx = LONG_MAX;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	lx = LONG_MIN;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	ulx = ULONG_MAX;
	printf(inputs[i++], ulx, ulx, ulx, ulx, ulx, ulx, ulx); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	i = 0;

	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	x = 0;
	printf(inputs[i++], x, x, x, x, x, x, x); 
	lx = LONG_MAX;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	lx = LONG_MIN;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	ulx = ULONG_MAX;
	printf(inputs[i++], ulx, ulx, ulx, ulx, ulx, ulx, ulx); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_length_mod_ll(void)
{	
	char *test_name = "test_length_mod_ll";
	int buf_size = 1000;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0, x;
	long	int lx;
	unsigned long int ulx;
	long long int	llx;
	unsigned long long int ullx;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i++] = "Argument is +-42: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is 0: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is LONG_MAX: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is LONG_MIN: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%ld|, |%ld|, |%li|, |%lo|, |%lu|, |%lx|, |%lX|\n";
	inputs[i++] = "Argument is LLONG_MAX: |%lld|, |%lld|, |%lldi|, |%lldo|, |%lldu|, |%lldx|, |%lldX|\n";
	inputs[i++] = "Argument is ULLONG_MAX: |%llu|, |%llu|, |%llui|, |%lluo|, |%lluu|, |%llux|, |%lluX|\n";
	i = 0;
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	x = 0;
	printf(inputs[i++], x, x, x, x, x, x, x); 
	lx = LONG_MAX;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	lx = LONG_MIN;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	ulx = ULONG_MAX;
	printf(inputs[i++], ulx, ulx, ulx, ulx, ulx, ulx, ulx); 
	llx = LLONG_MAX;
	printf(inputs[i++], llx, llx, llx, llx, llx, llx, llx); 
	ullx = ULLONG_MAX;
	printf(inputs[i++], ullx, ullx, ullx, ullx, ullx, ullx, ullx); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	printf(inputs[i++], 42, -42, 42, 42, 42, 42, 42); 
	x = 0;
	printf(inputs[i++], x, x, x, x, x, x, x); 
	lx = LONG_MAX;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	lx = LONG_MIN;
	printf(inputs[i++], lx, lx, lx, lx, lx, lx, lx); 
	ulx = ULONG_MAX;
	printf(inputs[i++], ulx, ulx, ulx, ulx, ulx, ulx, ulx); 
	llx = LLONG_MAX;
	printf(inputs[i++], llx, llx, llx, llx, llx, llx, llx); 
	ullx = ULLONG_MAX;
	printf(inputs[i++], ullx, ullx, ullx, ullx, ullx, ullx, ullx); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_length_mod_l_and_L_with_float(void)
{	
	char *test_name = "test_length_mod_l_and_L_with_float)";
	int buf_size = 10000;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0, x;
	float f;
	double df;
	long double ldf;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i++] = "Argument is +-42: |%lf|, |%Lf|, |%lf|, |%Lf|\n";
	inputs[i++] = "Argument is 0: |%lf|, |%Lf|, |%lf|, |%Lf|\n";
	inputs[i++] = "Argument is DBL_MAX: |%lf|, |%Lf|\n";
	inputs[i++] = "Argument is DBL_MIN: |%lf|, |%Lf|\n";
	inputs[i++] = "Argument is LDBL_MAX: |%lf|, |%Lf|\n";
	inputs[i++] = "Argument is LDBL_MIN: |%lf|, |%Lf|\n";
	i = 0;
	printf(inputs[i++], 42.0, 42.0, -42.0, -42.0); 
	f = 0;
	printf(inputs[i++], f, f, f, f); 
	df = DBL_MAX;
	printf(inputs[i++], df, df); 
	df = DBL_MIN;
	printf(inputs[i++], df, df); 
	ldf = LDBL_MAX;
	printf(inputs[i++], ldf, ldf); 
	ldf = LDBL_MIN;
	printf(inputs[i++], ldf, ldf); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	printf(inputs[i++], 42.0, 42.0, -42.0, -42.0); 
	f = 0;
	printf(inputs[i++], f, f, f, f); 
	df = DBL_MAX;
	printf(inputs[i++], df, df); 
	df = DBL_MIN;
	printf(inputs[i++], df, df); 
	ldf = LDBL_MAX;
	printf(inputs[i++], ldf, ldf); 
	ldf = LDBL_MIN;
	printf(inputs[i++], ldf, ldf); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_print_with_percent(int print_to_console)
{	
	char *test_name = "test_print_with_percent";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i++] = "What happens here %%d or here %d\n";
	i = 0;
	printf(inputs[i++], 42, 42); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	
	i = 0;
	inputs[i++] = "What happens here %%d or here %d\n";
	i = 0;
	printf(inputs[i++], 42, 42); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if(print_to_console == 1)
	{
		printf(res);
		printf(res2);
	}
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}
void usage(int argc, char *args)
{
	if(argc > 1 && *args != '1')
	{
		printf("Usage: without argument dumbs are written to results folder\nWith argument 1, printed to console");
		exit(-1);
	}
}

void test_some_edges()
{	
	char *test_name = "test_some_edges";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i++] = "Short or char?: %hhhd";
	i = 0;
	printf(inputs[i++], INT_MAX); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	
	i = 0;
	printf(inputs[i++]); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_d_and_i(void)
{	
	char *test_name = "test_d_and_i";
	int buf_size = 200000;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[2000];
	int i = 0, x;
	long	int lx;
	unsigned long int ulx;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	inputs[i++] = "Argument is +-42: |%ld|, |%ld|, |%li|\n";
	inputs[i++] = "Argument is 0: |%ld|, |%ld|, |%li|\n";
	inputs[i++] = "Argument is LONG_MAX: |%ld|, |%ld|, |%li|\n";
	inputs[i++] = "Argument is LONG_MIN: |%ld|, |%ld|, |%li|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%ld|, |%ld|, |%li|\n";
	inputs[i++] = "INT_MAX %d\n"	;
	inputs[i++] = "helllo %d\n" ;
 	inputs[i++] = "INT_MIN %d\n" ;
	inputs[i++] = "helllo %+d\n";
	inputs[i++] = "helllo %+d\n";
	inputs[i++] = "INT_MIN %+d\n";
	inputs[i++] = "INT_MAX %-d\n";
	inputs[i++] = "0 %-d\n";
	inputs[i++] = "INT_MIN %-d\n";
	inputs[i++] = "INT_MAX %#d\n";
	inputs[i++] = "helllo %#d\n";
	inputs[i++] = "helllo %#d\n";
	inputs[i++] = "helllo %0d\n";
	inputs[i++] = "helllo %0d\n";
	inputs[i++] = "helllo %0d\n";
	inputs[i++] = "helllo % d\n";
	inputs[i++] = "helllo % d\n";
	inputs[i++] = "helllo % d\n";
	inputs[i++] = "helllo |% 4d|\n";
	inputs[i++] = "helllo |%4 d|\n";
	inputs[i++] = "helllo |% 4d|\n";
	inputs[i++] = "helllo |% .4d|\n";
	inputs[i++] = "helllo |%.4 d|\n";
	inputs[i++] = "helllo |% .4d|\n";
	inputs[i++] = "helllo |%0.4d|\n";
	inputs[i++] = "helllo |%.40d|\n";
	inputs[i++] = "helllo |%0.4d|\n";
	inputs[i++] = "helllo |%0.4d|\n";
	inputs[i++] = "helllo |%.40d|\n";
	inputs[i++] = "helllo |%0.4d|\n";
	inputs[i++] = "helllo |%0-4d|\n";
	inputs[i++] = "helllo |%40-d|\n";
	inputs[i++] = "helllo |%-04d|\n";
	inputs[i++] = "helllo |%0-+4d|\n";
	inputs[i++] = "helllo |%4-+d|\n";
	inputs[i++] = "helllo |%+-04d|\n";
	inputs[i++] = "helllo |%0- 4d|\n";
	inputs[i++] = "helllo |%4- d|\n";
	inputs[i++] = "helllo |% -04d|\n";
	inputs[i++] = "helllo |%0- 4d|\n";
	inputs[i++] = "helllo |% -04d|\n";
	inputs[i++] = "helllo |%0- +4d|\n";
	inputs[i++] = "helllo |% -0+4d|\n" ;
	inputs[i++] = "helllo |%0 +4d|\n";
	inputs[i++] = "helllo |%0 +4d|\n";
	inputs[i++] = "helllo |%0 +4d|\n";
	inputs[i++] = "Argument is -3 |%.4d|\n";
	inputs[i++] = "Argument is zero |%.4d|\n";

   i = 0;
  printf(inputs[i++], 42, -42, 42); 
  x = 0;
  printf(inputs[i++], x, x, x); 
  lx = LONG_MAX;
  printf(inputs[i++], lx, lx, lx); 
  lx = LONG_MIN;
  printf(inputs[i++], lx, lx, lx); 
  ulx = ULONG_MAX;
  printf(inputs[i++], ulx, ulx, ulx); 
  printf(inputs[i++],	INT_MAX);
  printf(inputs[i++],	0);
  printf(inputs[i++],	INT_MIN);
  printf(inputs[i++],	 INT_MAX);
  printf(inputs[i++],	 0);
  printf(inputs[i++],	 INT_MIN);
  printf(inputs[i++],	 INT_MAX);
  printf(inputs[i++],	 0);
  printf(inputs[i++],	 INT_MIN);
  printf(inputs[i++],	 INT_MAX);
  printf(inputs[i++],	 0);
  printf(inputs[i++],	 INT_MIN);
  printf(inputs[i++],	 INT_MAX);
  printf(inputs[i++],	 0);
  printf(inputs[i++],	 INT_MIN);
  printf(inputs[i++],	 INT_MAX);
  printf(inputs[i++],	 0);
  printf(inputs[i++],	 INT_MIN);
  printf(inputs[i++],	3);
  printf(inputs[i++],	0);
  printf(inputs[i++],   	3);
  printf(inputs[i++],	 3);
  printf(inputs[i++],	 0);
  printf(inputs[i++],	 3);
  printf(inputs[i++],	 3);
  printf(inputs[i++],	 0);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 0);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 0);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 0);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 0);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3456789);
	printf(inputs[i++],	 -3456789);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 3);
	printf(inputs[i++],	 0);
	printf(inputs[i++],	 3);
	printf(inputs[i++],   	-3);
	printf(inputs[i++],		0);

  fflush(stdout);
  close(p[1]);
  if (pipe(p2) < 0)
  	exit(-1);
  dup2(p2[1], fileno(stdout)); 

  i = 0;
  ft_printf(inputs[i++], 42, -42, 42); 
  x = 0;
  ft_printf(inputs[i++], x, x, x); 
  lx = LONG_MAX;
  ft_printf(inputs[i++], lx, lx, lx); 
  lx = LONG_MIN;
  ft_printf(inputs[i++], lx, lx, lx); 
  ulx = ULONG_MAX;
  ft_printf(inputs[i++], ulx, ulx, ulx); 
  ft_printf(inputs[i++], INT_MAX);
  ft_printf(inputs[i++],0);
	ft_printf(inputs[i++],INT_MIN);
  ft_printf(inputs[i++], INT_MAX);
  ft_printf(inputs[i++], 0);
  ft_printf(inputs[i++], INT_MIN);
  ft_printf(inputs[i++], INT_MAX);
  ft_printf(inputs[i++], 0);
  ft_printf(inputs[i++], INT_MIN);
  ft_printf(inputs[i++], INT_MAX);
  ft_printf(inputs[i++], 0);
  ft_printf(inputs[i++], INT_MIN);
  ft_printf(inputs[i++], INT_MAX);
  ft_printf(inputs[i++], 0);
  ft_printf(inputs[i++], INT_MIN);
  ft_printf(inputs[i++], INT_MAX);
  ft_printf(inputs[i++], 0);
  ft_printf(inputs[i++], INT_MIN);
  ft_printf(inputs[i++],3);
  ft_printf(inputs[i++],0);
  ft_printf(inputs[i++], 	3);
  ft_printf(inputs[i++], 3);
  ft_printf(inputs[i++], 0);
  ft_printf(inputs[i++], 3);
  ft_printf(inputs[i++], 3);
  ft_printf(inputs[i++], 0);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 0);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 0);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 0);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 0);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3456789);
	ft_printf(inputs[i++], -3456789);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], 0);
	ft_printf(inputs[i++], 3);
	ft_printf(inputs[i++], -3);
	ft_printf(inputs[i++], 0);

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}


void wtf(void)
{	
	printf("%d\n", INT_MIN);

  	ft_printf("%d\n", INT_MIN);
	return ;
}

void test_unsigned_conversion_with_char()
{	
	char *test_name = "test_unsigned_conversion_with_char";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%hhu|\n";
	inputs[i++] = "Argument is CHAR_MIN: |%hhu|\n";
	inputs[i++] = "Argument is CHAR_MAX: |%hhu|\n";
	inputs[i++] = "Argument is UCHAR_MAX: |%hhu|\n";
	i = 0;
	printf(inputs[i++], 0); 
	printf(inputs[i++], CHAR_MIN); 
	printf(inputs[i++], CHAR_MAX); 
	printf(inputs[i++], UCHAR_MAX); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	
	i = 0;
	inputs[i++] = "Argument is ZERO: |%hhu|\n";
	inputs[i++] = "Argument is CHAR_MIN: |%hhu|\n";
	inputs[i++] = "Argument is CHAR_MAX: |%hhu|\n";
	inputs[i++] = "Argument is UCHAR_MAX: |%hhu|\n";
	i = 0;
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], CHAR_MIN); 
	ft_printf(inputs[i++], CHAR_MAX); 
	ft_printf(inputs[i++], UCHAR_MAX); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_unsigned_conversion_with_short()
{	
	char *test_name = "test_unsigned_conversion_with_short";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%hu|\n";
	inputs[i++] = "Argument is SHORT_MIN: |%hu|\n";
	inputs[i++] = "Argument is SHORT_MAX: |%hu|\n";
	inputs[i++] = "Argument is USHORT_MAX: |%hu|\n";
	i = 0;
	printf(inputs[i++], 0); 
	printf(inputs[i++], SHRT_MIN); 
	printf(inputs[i++], SHRT_MAX); 
	printf(inputs[i++], USHRT_MAX); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i++] = "Argument is ZERO: |%hu|\n";
	inputs[i++] = "Argument is SHORT_MIN: |%hu|\n";
	inputs[i++] = "Argument is SHORT_MAX: |%hu|\n";
	inputs[i++] = "Argument is USHORT_MAX: |%hu|\n";
	i = 0;
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], SHRT_MIN); 
	ft_printf(inputs[i++], SHRT_MAX); 
	ft_printf(inputs[i++], USHRT_MAX); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_unsigned_conversion_with_int()
{	
	char *test_name = "test_unsigned_conversion_with_int";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%u|\n";
	inputs[i++] = "Argument is INT_MIN: |%u|\n";
	inputs[i++] = "Argument is INT_MAX: |%u|\n";
	inputs[i++] = "Argument is UINT_MAX: |%u|\n";
	i = 0;
	printf(inputs[i++], 0); 
	printf(inputs[i++], INT_MIN); 
	printf(inputs[i++], INT_MAX); 
	printf(inputs[i++], UINT_MAX); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i++] = "Argument is ZERO: |%u|\n";
	inputs[i++] = "Argument is INT_MIN: |%u|\n";
	inputs[i++] = "Argument is INT_MAX: |%u|\n";
	inputs[i++] = "Argument is UINT_MAX: |%u|\n";
	i = 0;
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], INT_MIN); 
	ft_printf(inputs[i++], INT_MAX); 
	ft_printf(inputs[i++], UINT_MAX); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_unsigned_conversion_with_long()
{	
	char *test_name = "test_unsigned_conversion_with_long";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%lu|\n";
	inputs[i++] = "Argument is LONG_MIN: |%lu|\n";
	inputs[i++] = "Argument is LONG_MAX: |%lu|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%lu|\n";
	i = 0;
	printf(inputs[i++], 0); 
	printf(inputs[i++], LONG_MIN); 
	printf(inputs[i++], LONG_MAX); 
	printf(inputs[i++], ULONG_MAX); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i++] = "Argument is ZERO: |%lu|\n";
	inputs[i++] = "Argument is LONG_MIN: |%lu|\n";
	inputs[i++] = "Argument is LONG_MAX: |%lu|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%lu|\n";
	i = 0;
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], LONG_MIN); 
	ft_printf(inputs[i++], LONG_MAX); 
	ft_printf(inputs[i++], ULONG_MAX); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_unsigned_conversion_with_long_long()
{	
	char *test_name = "test_unsigned_conversion_with_long_long";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%llu|\n";
	inputs[i++] = "Argument is LLONG_MIN: |%llu|\n";
	inputs[i++] = "Argument is LLONG_MAX: |%llu|\n";
	inputs[i++] = "Argument is ULLONG_MAX: |%llu|\n";
	i = 0;
	printf(inputs[i++], 0); 
	printf(inputs[i++], LLONG_MIN); 
	printf(inputs[i++], LLONG_MAX); 
	printf(inputs[i++], ULLONG_MAX); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i++] = "Argument is ZERO: |%llu|\n";
	inputs[i++] = "Argument is LLONG_MIN: |%llu|\n";
	inputs[i++] = "Argument is LLONG_MAX: |%llu|\n";
	inputs[i++] = "Argument is ULLONG_MAX: |%llu|\n";
	i = 0;
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], LLONG_MIN); 
	ft_printf(inputs[i++], LLONG_MAX); 
	ft_printf(inputs[i++], ULLONG_MAX); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_unsigned_conversion_with_random_stuff()
{	
	char *test_name = "test_unsigned_conversion_with_random_stuff";
	int buf_size = 500;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[10];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%0+5llu|\n";
	inputs[i++] = "Argument is 42: |%-.14llu|\n";
	inputs[i++] = "Argument is 42: |%- 4u|\n";
	inputs[i++] = "Argument is -42: |%0-5u|\n";
	i = 0;
	printf(inputs[i++], 0); 
	printf(inputs[i++], 42); 
	printf(inputs[i++], 42); 
	printf(inputs[i++], 42); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

	i = 0;
	inputs[i++] = "Argument is ZERO: |%0+5llu|\n";
	inputs[i++] = "Argument is 42: |%-.14llu|\n";
	inputs[i++] = "Argument is 42: |%- 4u|\n";
	inputs[i++] = "Argument is -42: |%0-5u|\n";
	i = 0;
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], 42); 
	ft_printf(inputs[i++], 42); 
	ft_printf(inputs[i++], 42); 
	
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_octal()
{	
	char *test_name = "test_octal";
	int buf_size = 2000;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[100];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%.o|, |%#.o|, |%#-.o|, |%0.o|, |%-.o|, |%#0.o|\n";
	inputs[i++] = "Argument is ZERO: |%o|, |%#o|, |%#-o|, |%0o|, |%-o|, |%#0o|\n";
	inputs[i++] = "Argument is ZERO: |%4o|, |%#4o|, |%#-4o|, |%04o|, |%-4o|, |%#04o|\n";
	inputs[i++] = "Argument is UINT_MAX: |%4o|, |%0-o|, |%20.25o|, |% o|, |%+o|, |%+ o|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%4lo|, |%0-lo|, |%20.25lo|, |% lo|, |%+lo|, |%+ lo|\n";
	inputs[i++] = "Argument is ULLONG_MAX: |%4llo|, |%0-llo|, |%20.25llo|, |% llo|, |%+llo|, |%+ llo|\n";
	inputs[i++] = "Argument is LLONG_MIN: |%4llo|, |%0-llo|, |%20.25llo|, |% llo|, |%+llo|, |%+ llo|\n";
	inputs[i++] = "Argument is 3: |%4.o|\n"; 
	inputs[i++] = "Argument is 3: |%#6.7o|\n";
	inputs[i++] = "Argument is 0: |%#6.7o|\n";
	inputs[i++] = "Argument is 0: |%-.7o|\n";
	inputs[i++] = "Argument is 0: |%-#5.3o|\n";
	inputs[i++] = "Argument is 12: |%-#5.3o|\n";
	
	i = 0;
	printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	printf(inputs[i++], UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX); 
	printf(inputs[i++], ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX); 
	printf(inputs[i++], ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX); 
	printf(inputs[i++], LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN); 
	printf(inputs[i++], 3); 
	printf(inputs[i++], 3); 
	printf(inputs[i++], 0); 
	printf(inputs[i++], 0); 
	printf(inputs[i++], 0); 
	printf(inputs[i++], 12); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

i = 0;
	ft_printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	ft_printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	ft_printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	ft_printf(inputs[i++], UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX); 
	ft_printf(inputs[i++], ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX); 
	ft_printf(inputs[i++], ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX); 
	ft_printf(inputs[i++], LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN); 
	ft_printf(inputs[i++], 3); 
	ft_printf(inputs[i++], 3); 
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], 12); 
	

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_hexa()
{	
	char *test_name = "test_hexa";
	int buf_size = 2000;
	char res[buf_size];
	char res2[buf_size];
	char *inputs[100];
	int i = 0;
	int p[2], p2[2], stdout_bk;

	bzero(res, buf_size);
	bzero(res2, buf_size);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	i = 0;
	inputs[i++] = "Argument is ZERO: |%.o|, |%#.o|, |%#-.o|, |%0.o|, |%-.o|, |%#0.o|\n";
	inputs[i++] = "Argument is ZERO: |%o|, |%#o|, |%#-o|, |%0o|, |%-o|, |%#0o|\n";
	inputs[i++] = "Argument is ZERO: |%4o|, |%#4o|, |%#-4o|, |%04o|, |%-4o|, |%#04o|\n";
	inputs[i++] = "Argument is UINT_MAX: |%4o|, |%0-o|, |%20.25o|, |% o|, |%+o|, |%+ o|\n";
	inputs[i++] = "Argument is ULONG_MAX: |%4lo|, |%0-lo|, |%20.25lo|, |% lo|, |%+lo|, |%+ lo|\n";
	inputs[i++] = "Argument is ULLONG_MAX: |%4llo|, |%0-llo|, |%20.25llo|, |% llo|, |%+llo|, |%+ llo|\n";
	inputs[i++] = "Argument is LLONG_MIN: |%4llo|, |%0-llo|, |%20.25llo|, |% llo|, |%+llo|, |%+ llo|\n";
	inputs[i++] = "Argument is 3: |%4.o|\n"; 
	inputs[i++] = "Argument is 3: |%#6.7X|\n";
	inputs[i++] = "Argument is 0: |%#6.7X|\n";
	inputs[i++] = "Argument is 0: |%-.7x|\n";
	inputs[i++] = "Argument is 0: |%-#5.3x|\n";
	i = 0;
	printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	printf(inputs[i++], UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX); 
	printf(inputs[i++], ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX); 
	printf(inputs[i++], ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX); 
	printf(inputs[i++], LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN); 
	printf(inputs[i++], 3); 
	printf(inputs[i++], 3); 
	printf(inputs[i++], 0); 
	printf(inputs[i++], 0); 
	printf(inputs[i++], 0); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 

i = 0;
	ft_printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	ft_printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	ft_printf(inputs[i++], 0, 0, 0, 0, 0, 0); 
	ft_printf(inputs[i++], UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX); 
	ft_printf(inputs[i++], ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX); 
	ft_printf(inputs[i++], ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX, ULLONG_MAX); 
	ft_printf(inputs[i++], LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN, LLONG_MIN); 
	ft_printf(inputs[i++], 3); 
	ft_printf(inputs[i++], 3); 
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], 0); 
	ft_printf(inputs[i++], 0); 

	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, buf_size);
	read(p2[0], res2, buf_size);
	if (strcmp(res, res2) != 0)
	{
		dump_it(inputs, res, res2, test_name);
		printf("%-20s: FAIL\n", test_name);
		fflush(stdout);
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

void test_overflow()
{
	printf("Trying to overflow width and precision:\n");
	time_t the_end;
	int sec = 30; //30secs
	int ret = 0;

	the_end = time(NULL) + sec;
	while (time(NULL) < the_end)
	{
		ret = ft_printf("%2147483648d\n", 1234);
//		ret = ft_printf("%.2147483648d\n", 1234);
		if (ret > 0)
			break;
	}
	if (time(NULL) >= the_end)
		printf("Timeout after 30 seconds, is there protection? At least on linux it was set to INT_MAX\n");
}

// not needed when using generate_test_runner.rb
int main(int argc, char *args) {
//    UNITY_BEGIN();
//	usage(argc, args);
//    RUN_TEST(test_double_percent_sign);
//	test_hash_flag();
//	test_zero_flag();
// 	test_minus_flag();
//	test_space_flag();
//	test_plus_flag();
//	test_min_width();
//	test_precision();
//	test_length_mod_hh();
//	test_length_mod_h();
//	test_length_mod_l();
//	test_length_mod_ll();
//	test_length_mod_l_and_L_with_float();
//	test_print_with_percent(0);
//	test_some_edges();
//	test_d_and_i();
//	test_unsigned_conversion_with_char();
//	test_unsigned_conversion_with_short();
//	test_unsigned_conversion_with_int();
//	test_unsigned_conversion_with_long();
//	test_unsigned_conversion_with_long_long();
//	test_unsigned_conversion_with_random_stuff();
	test_octal();
//	test_hexa();

//	test_overflow();
	return (0);
//    return UNITY_END();
}

