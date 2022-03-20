#include "get_next_line.h"
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include "unity.h"
//#include "unity_internals.h"
#include "../ft_printf.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <limits.h>

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
	fd3 = open("results/errors.txt", O_RDWR | O_TRUNC | O_CREAT, 00644);
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
		return ;
	}
	close (p[0]);
	close (p2[0]);
	printf("%-20s: OK\n", test_name);
	fflush(stdout);
	return ;
}

// not needed when using generate_test_runner.rb
int main(void) {
//    UNITY_BEGIN();
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
	return (0);
//    return UNITY_END();
}

