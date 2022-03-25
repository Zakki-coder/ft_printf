/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jniemine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:51:33 by jniemine          #+#    #+#             */
/*   Updated: 2022/03/25 13:49:24 by jniemine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>	//DELTE
#include "ft_printf.h"



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
		if(n > f_str->width)
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
		if (f_str->precision < 0)
			f_str->precision = 0;
		while (**s == '.')
			++(*s);
		if(ft_isdigit(**s))
		{
			n = not_atoi(s);
			if(n > f_str->precision)
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


char *handle_width(t_fs *f_str, long long ll, char *nb, int len)
{
	int f;
	char *ret;

	f = f_str->flags;
	if (nb == NULL)
		exit(-1);
	if (len > f_str->width)
		f_str->width = len;
	if (f_str->precision > f_str->width)
		f_str->width = f_str->precision; //Behaves differently with float
	if (len == f_str->width && (ll < 0 || f & PLUS || f & SPACE)) //Protect for int overflow if you want
		++f_str->width;
	ret = (char *)ft_memalloc(sizeof(*ret) * f_str->width);	
	if (ret == NULL)
		exit(-1);
	return (ret);
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
	if (ll < 0)
		prefix = '-';
	else if (f & PLUS)
		prefix = '+';
	else if (f & SPACE)
		prefix = ' ';
	else
		return ;
	if ((f & MINUS) || (f & ZERO))
		*out = prefix;	
	else
		*(out + diff - 1) = prefix;
}

char *absolute_itoa(long long ll, int *len)
{
	long long	llong_min;
	long long	ll_the_second;
	char		*nb;

	*len = 0;
	ll_the_second = ll;
	if(ll_the_second == 0)
		++*len;
	while(ll_the_second)
	{
		++*len;
		ll_the_second /= 10;
	}
	llong_min = -9223372036854775807 - 1;
	if (ll == llong_min)
		return("9223372036854775808");
	if(ll < 0)
		ll *= -1;
	nb = ft_itoa(ll);
	return (nb);
}
	
char *process_conversion(t_fs *f_str, long long ll)
{
			//call either absolute_itoa, otoa, or xtoa
}


void print_di(t_fs *f_str, long long ll)
{
	char	*nb; //Remember to free, this number is going to be abs
	char	*out; //Remember to free
	int		len;
	int		diff;
			
	nb = absolute_itoa(ll, &len); //Make a function to decide which type of number is parsed, d , o or x, malloc protection is in handle_width
	out = handle_width(f_str, ll, nb, len); //Remember to free
	diff = f_str->width - len;
	ft_memset(out, ' ', f_str->width);
	if (f_str->flags & MINUS)
	{
		if (ll < 0 || f_str->flags & PLUS || f_str->flags & SPACE)
			diff = 1;
		else
			diff = 0;
	}
	if (!(f_str->flags & MINUS) && (f_str->flags & ZERO))
		ft_memset(out, '0', f_str->width);
	set_prefix(f_str, out, ll, diff);
	ft_memcpy((out + diff), nb, len);
	write(1, out, f_str->width);
	++f_str->str;
	free(nb);
	free(out);
}	

void print_conversion(t_fs *f_str, long long ll)
{
	char c;

	c = *f_str->str;
	//Add calls for the rest of diouxX, modify print_di to take care of things
	if (c == 'd' || c == 'i')
		print_di(f_str, (int)ll); //Does the casting work??
}
	
/* Never format string or argcs */
void format_fs(t_fs *f_str)
{
	f_str->return_n = 0;
	f_str->flags = 0;
	f_str->width = 0;
	f_str->precision = -1;
	f_str->modifier = 0;
	f_str->conversion = 0;
}

void parse_conversion(t_fs *f_str)
{
	long double		ld;
	long long int	ll;
	//str should be pointing to conversion
	//make a function which gets the argument from stack
	//Conver the value to octal-, hexa-, integer- or float string
	//One function to return long long and one to return long double
	if (*f_str->str != 'f')
	{
		ll = get_argument(f_str);
		print_conversion(f_str, ll);
	}
	else
		;
//	*f_str = (t_fs){.precision = -1}; //Does this work? Google why? New struct is created?
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
