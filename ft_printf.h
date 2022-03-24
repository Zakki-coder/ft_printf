/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jniemine <jniemine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 15:15:00 by jniemine          #+#    #+#             */
/*   Updated: 2022/03/24 14:35:45 by jniemine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>
# include "unistd.h"
# include <stdlib.h>

# define HASH	256
# define MINUS	64
# define ZERO	16
# define PLUS	4	
# define SPACE	1
# define LDBL	256
# define LLONG 	64
# define LONG	16
# define SHORT	4
# define CHAR	1	

typedef struct	s_format_string
{
	char	*str;
	va_list	argcs;
	int		return_n;
	int		flags;
	int		width;
	int		precision;
	int		modifier;
	int		conversion;
}				t_fs;

char *absolute_itoa(long long ll); //FOR DEBUG
void get_modifiers(t_fs *f_str);	//FOR DEBUG
void get_precision(t_fs *f_str); //FOR DEBUG
void get_width(t_fs *f_str);	//FOR DEBUG
void get_flags(t_fs *f_str); //FOR DEBUG
int is_conversion(char c);	//FOR DEBUG
int	ft_printf(char *str, ...);

#endif
