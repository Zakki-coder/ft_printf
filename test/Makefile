NAME =	ft_printf_unit_test

INC =	-I./Unity/src/ 	\
		-I./gnl/ 		\
		-I./gnl/libft/includes 		\
	
SRC =	./Unity/src/unity.c		\
		ft_printf_unit_test.c	\
		./gnl/get_next_line.c	\
		./gnl/libft/*.c	\

all:
	@rm -f ./results/*
	@gcc -o ${NAME} ${SRC} ${INC}

clean:
	@rm -f ${NAME}

re: clean all
