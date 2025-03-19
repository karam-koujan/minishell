Name = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
OBJ = main.o ./libft/ft_strlen.o ./libft/ft_strchr.o ./libft/ft_strnstr.o  src/error_checker/error_checker.o \
src/error_checker/error_checker_helper.o ./libft/ft_memcpy.o libft/ft_lstadd_back_bonus.o src/tokenizer/tokenizer.o \
./libft/ft_strdup.o ./libft/ft_strlcpy.o ./libft/ft_substr.o ./libft/ft_strncmp.o ./libft/ft_isalnum.o ./libft/ft_isdigit.o \
./libft/ft_isalpha.o

all : $(Name)

$(Name) : $(OBJ)
	$(CC) $(CFLAGS) -lreadline $^  -o $(Name)


%.o : %.c libft/libft.h includes/error_checker.h includes/minishell.H
	$(CC) $(CFLAGS) -c $< -o $@


clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(Name)

re : fclean all
