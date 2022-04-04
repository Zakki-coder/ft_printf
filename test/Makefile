NAME =	ft_printf_unit_test

INC =	-I./gnl/ 		\
		-I./../				\
		-I./../libft/	\
	
SRC =	ft_printf_unit_test.c	\
		./../ft_printf.c		\
		./gnl/get_next_line.c	\
		./../libft/*.c

all:
	@rm -f ./results/*
	@gcc -g -o ${NAME} ${SRC} ${INC}

clean:
	@rm -f ${NAME}

re: clean all
