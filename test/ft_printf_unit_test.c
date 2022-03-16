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

void test_hash_flag(void)
{	
	char *test_name = "test_hash_flag";
	char res[100];
	char res2[100];
	int p[2], p2[2], stdout_bk;

	bzero(res, 100);
	bzero(res2, 100);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//TODO Weird behaviour with the next printf %#f prints just zero or -nan
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("With hash: %#o and %#x and %#X and %#f and %#.0f and %#f\n", 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 100);
	read(p2[0], res2, 100);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected: %s", res);
		printf("Was:	  %s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_zero_flag(void)
{	
	char *test_name = "test_zero_flag";
	char res[200];
	char res2[200];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//The value is zero padded, - flag overrides 0 flag, with precision for diouxX 0 flag is ignored, for others undefined
	printf("With zero flag: %04d, %04i, %04o, %04u, %04x, %04X, %04f\n"
		"With precision: %04.2d, %04.2i, %04.2o, %04.2u, %04.2x, %04.2X, %04.2f\n"
		"With - flag   : %0-4d, %-04i, %0-4o, %-04u, %0-4x, %-04X, %0-4f\n", 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("With zero flag: %04d, %04i, %04o, %04u, %04x, %04X, %04f\n"
		"With precision: %04.2d, %04.2i, %04.2o, %04.2u, %04.2x, %04.2X, %04.2f\n"
		"With - flag   : %0-4d, %-04i, %0-4o, %-04u, %0-4x, %-04X, %0-4f\n", 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_minus_flag(void)
{	
	char *test_name = "test_minus_flag";
	char res[200];
	char res2[200];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//- overriding 0 is tested in the zero test
	printf("With - flag: |%-4d|, |%-4i|, |%-4o|, |%-4u|, |%-4x|, |%-4X|, |%-8.1f|\n", 42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("With - flag: |%-4d|, |%-4i|, |%-4o|, |%-4u|, |%-4x|, |%-4X|, |%-8.1f|\n", 42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_space_flag(void)
{	
	char *test_name = "test_space_flag";
	char res[200];
	char res2[200];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//Does nothing with ouxX because those are unsigned conversions not signed!
	printf("With ' ' flag: |% d|, |% d|, |% i|, |% o|, |% u|, |% x|, |% X|, |% f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("With ' ' flag: |% d|, |% d|, |% i|, |% o|, |% u|, |% x|, |% X|, |% f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_plus_flag(void)
{	
	char *test_name = "test_plus_flag";
	char res[200];
	char res2[200];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//Does nothing with ouxX because those are unsigned conversions not signed!
	printf("With + flag: |%+d|, |%+d|, |%+i|, |%+o|, |%+u|, |%+x|, |%+X|, |%+f|, |%+f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("With ' ' flag: |% d|, |% d|, |% i|, |% o|, |% u|, |% x|, |% X|, |% f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_min_width(void)
{	
	char *test_name = "test_min_width";
	char res[200];
	char res2[200];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	printf("Width test: |%6d|, |%6d|, |%6i|, |%6o|, |%6u|, |%6x|, |%6X|, |%8.2f|, |%8.2f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	printf("Truncation: |%0d|, |%1d|, |%1i|, |%1o|, |%1u|, |%1x|, |%1X|, |%1f|, |%0f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("Width test: |%6d|, |%6d|, |%6i|, |%6o|, |%6u|, |%6x|, |%6X|, |%8.2f|, |%8.2f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	printf("Truncation: |%0d|, |%1d|, |%1i|, |%1o|, |%1u|, |%1x|, |%1X|, |%1f|, |%0f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_precision(void)
{	
	char *test_name = "test_precision";
	char res[200];
	char res2[200];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	printf("precision test: |%.4d|, |%.4d|, |%.4i|, |%.4o|, |%.4u|, |%.4x|, |%.4X|, |%.2f|, |%.2f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	printf("for string    : |%.s|, |%.3s|, |%.10s|\n", "Wisdom", "Wisdom", "Wisdom"); 
	printf("With just '.' : |%.d|, |%.d|, |%.i|, |%.o|, |%.u|, |%.x|, |%.X|, |%.f|, |%.f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("precision test: |%.4d|, |%.4d|, |%.4i|, |%.4o|, |%.4u|, |%.4x|, |%.4X|, |%.2f|, |%.2f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	printf("for string    : |%.s|, |%.3s|, |%.10s|\n", "Wisdom", "Wisdom", "Wisdom"); 
	printf("With just '.' : |%.d|, |%.d|, |%.i|, |%.o|, |%.u|, |%.x|, |%.X|, |%.f|, |%.f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 200);
	read(p2[0], res2, 200);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}

void test_length_mod_hh(void)
{	
	char *test_name = "test_length_mod_hh";
	char res[500];
	char res2[500];
	int p[2], p2[2], stdout_bk;

	bzero(res, 200);
	bzero(res2, 200);
	if (pipe(p) < 0)
		exit(-1);
	stdout_bk = dup(fileno(stdout));
	dup2(p[1], fileno(stdout)); 
	//When integer is given as argument, output is the value which corresponds to conversion. So hh does nothing.
	//With overflow and underflow, trust casting, it causes UB. With overflows then output is argument mod 128, sometimes in decimal form.
	printf("length mod hh: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|, |%hhf|, |%hhf|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	signed char c = -1;
	//with negative value, just the value gets printed. 
	printf("length mod hh: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|, |%hhf|\n", c, c, c, c, c, c, c, c, c); 
	unsigned char ch = 'c';
	//float prints just zeroes, assumably undefined behavior.
	printf("length mod hh: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|, |%hhf|\n", ch, ch, ch, ch, ch, ch, ch, ch, ch); 
	ch = 0;
	printf("length mod hh: |%hhd|, |%hhd|, |%hhi|, |%hho|, |%hhu|, |%hhx|, |%hhX|, |%hhf|\n", ch, ch, ch, ch, ch, ch, ch, ch, ch); 
	fflush(stdout);
	fflush(stdout);
	close(p[1]);
	if (pipe(p2) < 0)
		exit(-1);
	dup2(p2[1], fileno(stdout)); 
	printf("precision test: |%.4d|, |%.4d|, |%.4i|, |%.4o|, |%.4u|, |%.4x|, |%.4X|, |%.2f|, |%.2f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	printf("for string    : |%.s|, |%.3s|, |%.10s|\n", "Wisdom", "Wisdom", "Wisdom"); 
	printf("With just '.' : |%.d|, |%.d|, |%.i|, |%.o|, |%.u|, |%.x|, |%.X|, |%.f|, |%.f|\n", 42, -42, 42, 42, 42, 42, 42, 42.0, -42.0); 
	fflush(stdout);
	close(p2[1]);
	dup2(stdout_bk, fileno(stdout));
	read(p[0], res, 500);
	read(p2[0], res2, 500);
	if (strcmp(res, res2) != 0)
	{
		printf("%s:\n", test_name);
		printf("Expected:\n%s\n", res);
		printf("Was:\n%s", res2);
	}else{
		printf("OK\n");
	}
	close (p[0]);
	close (p2[0]);
	return ;
}


// not needed when using generate_test_runner.rb
int main(void) {
//    UNITY_BEGIN();
//    RUN_TEST(test_double_percent_sign);
//    test_plus_flag();
//	test_hash_flag();
//	test_zero_flag();
// 	test_minus_flag();
//	test_space_flag();
//	test_min_width();
//	test_precision();
	test_length_mod_hh();
	return (0);
//    return UNITY_END();
}

