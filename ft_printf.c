/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jniemine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:51:33 by jniemine          #+#    #+#             */
/*   Updated: 2022/03/22 15:02:17 by jniemine         ###   ########.fr       */
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
		if(*str != '%' || (*str == '%' && *(str + 1) == '%') 
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
	++s; //for %
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

void get_width(t_fs *f_str)
{
	int n;
	
	n = 0;
	//If isdigit == TRUE, get value for width
	while (ft_isdigit(*s))
	{
		n *= 10;
		n += *s - '0';	
		++s;
	}
	f_str->width = n;
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
		while(!is_conversion(*str))
		{
			get_flags(&f_str);
	//Loop for width(number), precision(.number), length mod(letter) until conversion or \0 is encountered
			get_width(&f_str);
	//If width is found call get_width.
	//If precision is found call get_precision
	//If letter is found, check that its not conversion and call get_length_mod
	//If conversion is encountered, call get_arg.
	//If \0 is encountered, return failure and print everything as string
		}
	}
}

int	ft_printf(char *str, ...)
{
	t_fs	f_str = {.return_n = 0};

	f_str.str = str;
	va_start(f_str.argcs, str);
	parser(f_str);
	va_end(f_str.argcs);
	return(f_str.return_n);
}
