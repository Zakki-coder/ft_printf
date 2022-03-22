/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jniemine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:51:33 by jniemine          #+#    #+#             */
/*   Updated: 2022/03/22 20:34:59 by jniemine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>	//DELTE
#include "ft_printf.h"



int print_chars(t_fs *f_str)
{
	char *str;
	int n;

	n = f_str->return_n;
	str = f_str->str;
	while(*str != '\0')
	{
		//TODO test edge cases here
		if(*str != '%' || (*(str + 1) != '\0' && *str == '%' && *(str + 1) == '%') 
			|| (n > 0 && *(str - 1) == '%' && *str == '%'))
			write(1, str, 1);
		else
			return (1);
		++n;
		++str;
	}
	f_str->return_n += n;
	if (*str == '\0')
		return (-1);
	return (1);
}

void get_flags(t_fs *f_str)
{
	int *flags;
	char *s;

	s = f_str->str;
	flags = &f_str->flags;
	//Traverse in a loop until no flag is ecountered, return int where flag have been encoded as bits	
	while(*s == '#' || *s == '-' || *s == '0' || *s == '+' || *s == ' ')
	{
		if (*s == '#')	
			*flags |= HASH;
		if (*s == '-')
			*flags |= MINUS;
		if (*s == '0')
			*flags |= ZERO;
		if (*s == '+')
			*flags |= PLUS;
		if (*s == ' ')
			*flags |= SPACE;
		++s;
	}
}

int is_conversion(char c)
{
	return (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i'
			|| c == 'o' || c == 'u' || c == 'x' || c == 'X');
}

int not_atoi(char **s)
{
	int n;

	n = 0;
	while (ft_isdigit(**s))
	{
		n *= 10;
		n += **s - '0';	
		++(*s);
	}
	return(n);
}
	
void get_width(t_fs *f_str)
{
	int n;
	char **s;
	
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
	char **s;

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
	char **s;

	s = &f_str->str;
	//loop until largest modifier is found, or conversion char or null is found
	//No need to worry about over or underflow because str is limited with % and \0
	//What should happen with in case like hhh?
	while (is_modifier(**s))
	{
		if (**s == 'L')
			f_str->modifier |= LDBL;
		if (**s == 'l' && *(*s + 1) == 'l')
			f_str->modifier |= LLONG;
		if (**s == 'l' && *(*s - 1) != 'l' && *(*s + 1) != 'l')
			f_str->modifier |= LONG;
		if (**s == 'h' && *(*s - 1) != 'h' && *(*s + 1) != 'h')
			f_str->modifier |= SHORT;
		if (**s == 'h' && *(*s + 1) == 'h')
			f_str->modifier |= CHAR;
		++(*s);
	}
}
		
void parser(t_fs f_str)
{
	char *str;
	int *n;

	n = &f_str.return_n;
	str = f_str.str;
	while (*str != '\0')
	{
	//Traverse and print fs until %, with print_chars then send to get_flags
	if(print_chars(&f_str) == -1)
		return ;
		while(!is_conversion(*str) && *str != '\0')
		{
			++str;
			get_flags(&f_str);
	//Loop for width(number), precision(.number), length mod(letter) until conversion or \0 is encountered
			get_width(&f_str);
			get_precision(&f_str);
			get_modifiers(&f_str);
	//If letter is found, check that its not conversion and call get_length_mod
	//If conversion is encountered, call get_arg.
	//If \0 is encountered, return failure and print everything as string
		}
	}
}

int	ft_printf(char *str, ...)
{
	//TEST that implicit formatting works
	t_fs	f_str = {.precision = -1};

	f_str.str = str;
	va_start(f_str.argcs, str);
	parser(f_str);
	va_end(f_str.argcs);
	return(f_str.return_n);
}
