/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jniemine <jniemine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 15:15:00 by jniemine          #+#    #+#             */
/*   Updated: 2022/03/22 14:47:58 by jniemine         ###   ########.fr       */
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

typedef struct	s_format_string
{
	char	*str;
	va_list	argcs;
	int		return_n;
	int		flags;
	int		width;
	int		precision;
	int		modifiers;
	int		conversion;
}				t_fs;

void get_flags(t_fs *f_str); //FOR DEBUG
int is_conversion(char c);	//FOR DEBUG
int	ft_printf(char *str, ...);

#endif
