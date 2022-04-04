/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jniemine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:51:33 by jniemine          #+#    #+#             */
/*   Updated: 2022/04/04 21:30:28 by jniemine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>	//DELTE
#include "ft_printf.h"

int has_prefix(t_fs *f_str);

int print_chars(t_fs *f_str)
{
	const char **str;
	int n;

	n = f_str->return_n;
	str = &f_str->str;
	while(**str != '\0')
	{
		//TODO test edge cases here
		if(**str != '%' || (*(*str + 1) != '\0' && **str == '%' && *(*str + 1) == '%') 
			|| (n > 0 && *(*str - 1) == '%' && **str == '%'))
			write(1, *str, 1);
		else
			return (1);
		++n;
		++(*str);
	}
	f_str->return_n += n;	//Well this doesnt work
	if (**str == '\0')
		return (-1);
	return (1);
}

void get_flags(t_fs *f_str)
{
	int *flags;
	const char **s;

	s = &f_str->str;
	flags = &f_str->flags;
	//Traverse in a loop until no flag is ecountered, return int where flag have been encoded as bits	
	while(**s == '#' || **s == '-' || **s == '0' || **s == '+' || **s == ' ')
	{
		if (**s == '#')	
			*flags |= HASH;
		if (**s == '-')
			*flags |= MINUS;
		if (**s == '0')
			*flags |= ZERO;
		if (**s == '+')
			*flags |= PLUS;
		if (**s == ' ')
			*flags |= SPACE;
		++(*s);
	}
}

int is_conversion(char c)
{
	return (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i'
			|| c == 'o' || c == 'u' || c == 'x' || c == 'X');
}

int not_atoi(const char **s)
{
	int n;

	n = 0;
	while (ft_isdigit(**s))
	{
		n *= 10;
		n += **s - '0';	
		++(*s);
	}
	--(*s);
	return(n);
}
	
void get_width(t_fs *f_str)
{
	int n;
	const char **s;
	
	s = &f_str->str;
	n = 0;
	if (ft_isdigit(**s))
	{
		n = not_atoi(s);
		if(n > f_str->width && n <= MAX_INT) //At least on linux this is the limit
			f_str->width = n;
	}
}

/* precision has been initialized to -1 and changes to at least to zero if . is found */
void get_precision(t_fs *f_str)
{
	int n;
	const char **s;

	n = 0;
	s = &f_str->str;
	if(**s == '.')
	{
		f_str->is_precision = 1;
		while (**s == '.')
			++(*s);
		if(ft_isdigit(**s))
		{
			n = not_atoi(s);
			if(n > f_str->precision && n <= MAX_INT)
				f_str->precision = n;
		}
	}
}

/* ll or hh doesnt need to be checked because l and h will catch those */
int is_modifier(char c)
{
	return (c == 'L' || c == 'l' || c == 'h');
}

void get_modifiers(t_fs *f_str)
{
	const char **s;

	s = &f_str->str;
	//loop until largest modifier is found, or conversion char or null is found
	//No need to worry about over or underflow because str is limited with % and \0
	//What should happen in case like hhh?
	//h is more significant is my assumption. So if hhh then at least h.
	while (is_modifier(**s))
	{
		if (**s == 'L')
			f_str->modifier |= LDBL;
		if (**s == 'l' && *(*s + 1) == 'l')
			f_str->modifier |= LLONG;
		if (**s == 'l' && *(*s - 1) != 'l' && *(*s + 1) != 'l')
			f_str->modifier |= LONG;
		if (**s == 'h' && *(*s + 1) == 'h'
			&& *(*s - 1) != 'h' && *(*s + 2) != 'h' && (*s)++)
			f_str->modifier |= CHAR;
		else if(**s == 'h')
			f_str->modifier |= SHORT;
		++(*s);
	}
}

long long get_argument(t_fs *f_str)
{
	int m;
	long long arg;

	m = f_str->modifier;
	if (m & LDBL)
		arg = va_arg(f_str->argcs, long double);//call get_ldbl, which gets the value How the original does it, is there SEGFAULT here?
	else if (m & LLONG)
		arg = va_arg(f_str->argcs, long long);
	else if (m & LONG)
		arg = va_arg(f_str->argcs, long);
	else
		arg = va_arg(f_str->argcs, int);
	return (arg);
}
/* Handle width is used multiple places */

void handle_width(t_fs *f_str, int len)
{
	int f;
	char *ret;

	f = f_str->flags;
	if (len > f_str->width)
		f_str->width = len;
	if (f_str->precision > f_str->width)
		f_str->width = f_str->precision; //Behaves differently with float
	if (len == f_str->width && has_prefix(f_str))
		if (*f_str->str == 'd' || *f_str->str == 'i'
		&& f_str->width < MAX_INT) //At least on linux this is the limit.
			++f_str->width;
	if ((!(f & MINUS)) && has_prefix(f_str) && f_str->precision == f_str->width)
		++f_str->width;
}
/* itodiutoa starts */
static int	nb_length(unsigned long long nb)
{
	int	n;

	n = 0;
	if (nb == 0)
		return (1);
	while (nb)
	{
		++n;
		nb /= 10;
	}
	return (n);
}

int has_prefix(t_fs *f_str)
{
	int f;

	f = f_str->flags;
	return (f_str->neg || f & SPACE || f & PLUS);
}



int is_signed(char c)
{
	return (c == 'd' || c == 'i');
}

void set_prefix(t_fs *f_str, char *out, long long ll, int diff)
{
	int f;
	char prefix;

	f = f_str->flags;
	if (!is_signed(*f_str->str))
		return ;	
	if (f_str->neg)
		prefix = '-';
	else if (f & PLUS)
		prefix = '+';
	else if (f & SPACE)
		prefix = ' ';
	else
		return ;
	if ((f & MINUS) || (f & ZERO) || f_str->is_precision)
		*out = prefix;	
	else
		*(out + diff - 1) = prefix;
}

char *not_itoa(char *out, unsigned long long nb, int len, int diff)
{
	unsigned long long	ll;

	ll = 0;
	while (len > 0)
	{
		ll = nb - ((nb / 10) * 10);
		nb /= 10;
		*(out + diff + --len) = ll + '0';
	}	
	return (out);
}

unsigned long long convert_from_negativity(t_fs *f_str, long long ll)
{
	unsigned long long ull;

	ull = 0;
	if (*f_str->str != 'u' && ll < 0)
	{
		f_str->neg = 1;
		if (ll == -9223372036854775807 - 1)
			ull = (unsigned long long)9223372036854775807 + 1;
		else
			ull = ll * -1;
		return (ull);
	}
	ull = ll;
	return (ull);
}
//Precision guarantees the number of digits, so if there is precision and prefix, then +1 width
void itodiutoa(t_fs *f_str, long long ll)
{
	char	*out; //Remember to free
	int		len;
	int		diff;
	unsigned long long ull;
			
	ull = convert_from_negativity(f_str, ll);
	len = nb_length(ull);
	handle_width(f_str, len); //Remember to free
	diff = f_str->width - len;
	out = (char *)ft_memalloc(sizeof(*out) * f_str->width);
	if (out == NULL)
		exit (-1);
	ft_memset(out, ' ', f_str->width);
	if (f_str->flags & MINUS)
	{
		if (ll < 0 || f_str->flags & PLUS || f_str->flags & SPACE)
			diff = 1;
		else
			diff = 0;
	}
	if (!(f_str->flags & MINUS) && f_str->is_precision)
		ft_memset(out + (f_str->width - f_str->precision), '0', f_str->precision);
	else if (!(f_str->flags & MINUS) && f_str->flags & ZERO)
		ft_memset(out, '0', f_str->width);
	out = not_itoa(out, ull, len, diff); //Make a function to decide which type of number is parsed, d , o or x, malloc protection is in handle_width
	set_prefix(f_str, out, ll, diff);
	write(1, out, f_str->width);
	++f_str->str;
	free(out);
}
/* itodiutoa ends */

/* At the moment only used with octal */
void print_zeroes(int len)
{
	while (len > 0)
	{
		ft_putnbr(0);
		--len;
	}
}

/* At the moment only used with octal */
void print_spaces(int len)
{
	while (len > 0)
	{
		ft_putchar(' ');
		--len;
	}
}
/* Octa Starts */
/*Argument type is uint because that was the one which gave correct results on my linux*/
unsigned int octal_len(unsigned long long ull)
{
	char s[100];
	int i;

	i = 0;
	ft_bzero(s, 100);
	if (ull == 0)
		return (1);
	while(ull > 0)
	{
		s[i] = (ull % 8) + '0';
		ull /= 8;
		++i;
	}
	return (ft_strlen(s));
}

void str_reverse(char *s)
{
	int len;
	int i;
	char temp[100];

	bzero(temp, 100);
	len = ft_strlen(s) - 1;
	i = 0;
	while (len >= 0)
	{
		temp[i] = s[len];
		++i;
		--len;
	}
	ft_putstr(temp);
}

unsigned int octal_print(unsigned long long ull)
{
	char s[100];
	int i;

	if (ull == 0)
	{
		ft_putnbr(0);
		return (0);
	}
	i = 0;
	ft_bzero(s, 100);
	while(ull > 0)
	{
		s[i] = (ull % 8) + '0';
		ull /= 8;
		++i;
	}
	str_reverse(s);
	return (ft_strlen(s));
}

void right_adjusted_octal(t_fs *fs, unsigned long long ull, int len)
{
	if (fs->precision > 0)
	{
		print_spaces(fs->width - fs->precision - len);
		if (ull == 0)
			print_zeroes(fs->precision);
		else
			print_zeroes(fs->precision - len);
	}
	else if (!fs->is_precision)
	{			
		if (!(fs->flags & ZERO))
			print_spaces(fs->width - fs->precision - len);
		if (fs->flags & HASH)
			print_zeroes(1);
		if (fs->flags & ZERO)
			print_zeroes(fs->width - len);
	}
	else if (fs->is_precision)
	{		
		print_spaces(fs->width - fs->precision - len);
		if (fs->flags & HASH && ++len)
			print_zeroes(1);
	}
	if (ull > 0 || (ull == 0 && !fs->is_precision && !(fs->flags & HASH)))
		octal_print(ull);
}
/* Number of zeroes = precision - number length. If number is nonzero.
 * If precision is not given and zero flag is on. Number of zeroes = width - number length
 */
void itootoa(t_fs *f_str, unsigned long long ull)
{
	int len;
	int width;
	int precision;

	width = f_str->width;
	precision = f_str->precision;
	//Always print zero except when precision is 0 and there is no hash.
	/* Create number, calculate width > precision > number length, choose largest */
	len = octal_len(ull);
	handle_width(f_str, len);
	if (!(f_str->flags & MINUS))
		right_adjusted_octal(f_str, ull, len);
	else
	{
		/* With # prefix with zero, test with zero and 0 precision. */
		if (f_str->flags & HASH || (ull == 0 && !(f_str->flags & HASH)
			&& !(f_str->is_precision && f_str->precision == 0)))
			print_zeroes(1);
		if (len > f_str->precision)
			f_str->precision = len;
		if (ull > 0)
		{
			if (f_str->flags & HASH)
				++len;
			print_zeroes(f_str->precision - len);
		}
		else if (ull == 0 && f_str->precision > 0)
			print_zeroes(f_str->precision - len);
		if (ull > 0)
			octal_print(ull);
		if (f_str->width > f_str->precision && f_str->is_precision);
			print_spaces(f_str->width - f_str->precision);
	}
	++f_str->str;
}
/* Octal ends here */
/* Every conversion has a width for the whole, length for just the digits and offset for where to print it the field */

/* itoxa starts */

unsigned int hexa_len(unsigned long long ull)
{
	char s[100];
	int i;

	i = 0;
	ft_bzero(s, 100);
	if (ull == 0)
		return (1);
	while(ull > 0)
	{
		if (ull % 16 > 9)
			s[i] = (ull % 16) - 9 + '@';
		else
			s[i] = ull % 16 + '0';
		ull /= 16;
		++i;
	}
	printf("%s\n", s);
	return (ft_strlen(s));
}

void put_hexa_prefix(t_fs *fs)
{
	char l_case;

	l_case = *fs->str;
	if (l_case == 'X')
		ft_putstr("0X");
	if (l_case == 'x')
		ft_putstr("0x");
}

void hexa_print(unsigned long long ull)
{
	char s[100];
	int i;

	i = 0;
	ft_bzero(s, 100);
	if (ull == 0)
		return ;
	while(ull > 0)
	{
		if (ull % 16 > 9)
			s[i] = (ull % 16) - 9 + '@';
		else
			s[i] = ull % 16 + '0';
		ull /= 16;
		++i;
	}
	str_reverse(s);
}
/* Dont print prefix if ull == 0 and precision is given*/
void right_adjusted_hexa(t_fs *fs, unsigned long long ull, int len)
{
	if (fs->precision > 0)
	{
		print_spaces(fs->width - fs->precision - len);
		if (ull > 0)
			put_hexa_prefix(fs);	
		print_zeroes(fs->precision - len);
	}
	else if (!fs->is_precision)
	{			
		if (!(fs->flags & ZERO))
			print_spaces(fs->width - len);
		put_hexa_prefix(fs);	
		if (fs->flags & ZERO)
			print_zeroes(fs->width - len);
	}
	else if (fs->is_precision)
	{		
		print_spaces(fs->width - fs->precision - len);
		if (ull > 0)
			put_hexa_prefix(fs);
	}
	if (ull > 0 || (ull == 0 && fs->precision > 0))
		hexa_print(ull);
}
void itoxa(t_fs *f_str, unsigned long long ull)
{
	/* For x produce output with lowercase, for X with UPPERCASE */
	/* precision is the minimum numbers of digits to appear, test with zero padd */
	/* # flag prefixes with 0x or 0X, for NONZERO!!!! */
	/* 0 flag padds with zeroes on the left */
	/* - flag left adjustes and overrides 0 flag */
	/* NO SPACE OR + FLAG */
	int len;
	int width;
	int precision;

	width = f_str->width;
	precision = f_str->precision;
	//Always print zero except when precision is 0 and there is no hash.
	/* Create number, calculate width > precision > number length, choose largest */
	len = hexa_len(ull);
	handle_width(f_str, len);
	if (!(f_str->flags & MINUS))
		right_adjusted_hexa(f_str, ull, len);
	else
	{
		/* With # prefix with zero, test with zero and 0 precision. */
		/* When left adjusted precision is zero padding + len */
		if (f_str->flags & HASH && !(ull == 0 && f_str->is_precision))
			put_hexa_prefix(f_str);
		else if (ull == 0 && f_str->is_precision)
		{
			print_zeroes(f_str->precision);
			print_spaces(f_str->width - f_str->precision);
		}
		hexa_print(ull);
		print_spaces(f_str->width - len);
	}
	++f_str->str;
}

void function_dispatcher(t_fs *f_str, long long ll)
{
			//call either absolute_itoa, otoa, or xtoa, or the unsigned one
	if(*f_str->str == 'd' || *f_str->str == 'i' || *f_str->str == 'u')	
		itodiutoa(f_str, ll);
	if (*f_str->str == 'o')
		itootoa(f_str, ll);
//	if (*f_str->str == 'x')
//		itoxa(f_str, ll);
}

unsigned long long cast_to_modifier_u(t_fs *f_str, unsigned long long ll)
{
	int m;

	m = f_str->modifier;
	if (m & LLONG)
		return((unsigned long long)ll);//print_di(f_str, (long long)ll);
	else if (m & LONG)
		return((unsigned long)ll);//print_di(f_str, (long)ll);
	else if (m & SHORT)
		return((unsigned short)ll);//print_di(f_str, (short)ll);
	else if (m & CHAR)
		return((unsigned char)ll);//print_di(f_str, (char)ll);
	return((unsigned int)ll);//print_di(f_str, (int)ll);
}

long long cast_to_modifier(t_fs *f_str, long long ll)
{
	int m;

	m = f_str->modifier;
	//Add calls for the rest of diouxX, modify print_di to take care of things
	//Modifier decides casting, diouxX is just the format
	if (m & LLONG)
		return((long long)ll);//print_di(f_str, (long long)ll);
	else if (m & LONG)
		return((long)ll);//print_di(f_str, (long)ll);
	else if (m & SHORT)
		return((short)ll);//print_di(f_str, (short)ll);
	else if (m & CHAR)
		return((char)ll);//print_di(f_str, (char)ll);
	return((int)ll);//print_di(f_str, (int)ll);
}
	
/* Never format string or argcs */
void format_fs(t_fs *f_str)
{
	f_str->return_n = 0;
	f_str->flags = 0;
	f_str->width = 0;
	f_str->precision = 0;
	f_str->is_precision = 0;
	f_str->modifier = 0;
	f_str->conversion = 0;
	f_str->neg = 0;
}

unsigned long long get_argument_u(t_fs *f_str)
{
	int m;
	unsigned long long arg;

	m = f_str->modifier;
	if (m & LDBL)
		arg = va_arg(f_str->argcs, long double);//call get_ldbl, which gets the value How the original does it, is there SEGFAULT here?
	else if (m & LLONG)
		arg = va_arg(f_str->argcs, long long);
	else if (m & LONG)
		arg = va_arg(f_str->argcs, long);
	else
		arg = va_arg(f_str->argcs, int);
	return (arg);
}

void parse_conversion(t_fs *f_str)
{
	long double			ld;
	long long int		ll;
	unsigned long long	ull;
	//str should be pointing to conversion
	//make a function which gets the argument from stack
	//Conver the value to octal-, hexa-, integer- or float string
	//One function to return long long and one to return long double
	if (*f_str->str != 'f')
	{
		if (*f_str->str != 'd' && *f_str->str != 'i')
		{
			ull = get_argument_u(f_str);
			ull = cast_to_modifier_u(f_str, ull);
			function_dispatcher(f_str, ull);
		}
		else
		{
			ll = get_argument(f_str);
			ll = cast_to_modifier(f_str, ll);
			function_dispatcher(f_str, ll);
		}

	}
	else
		;
	format_fs(f_str);
}

void parser(t_fs f_str)
{
	const char **str;
	int *n;

	n = &f_str.return_n;
	str = &f_str.str;
	while (**str != '\0')
	{
	//Traverse and print fs until %, with print_chars then send to get_flags
	if(print_chars(&f_str) == -1)
		return ;
		while(!is_conversion(**str) && **str != '\0')
		{
			++(*str);
			get_flags(&f_str);
	//Loop for width(number), precision(.number), length mod(letter) until conversion or \0 is encountered
			get_width(&f_str);
			get_precision(&f_str);
			get_modifiers(&f_str);
		}
		//Print the first conversion out and reset struct
		if (is_conversion(**str))
			parse_conversion(&f_str);
	}
	//If we find \0 before conversion, nothing gets printed int the '%'-'\0' range, is it correcto?
}

int	ft_printf(const char *str, ...)
{
	//TEST that implicit formatting works
	t_fs	f_str;

	format_fs(&f_str);
	f_str.str = str;
	va_start(f_str.argcs, str);
	parser(f_str);
	va_end(f_str.argcs);
	return(f_str.return_n);
}
