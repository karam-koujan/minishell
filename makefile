Name = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
OBJ = main.o ./lib/libft/ft_strlen.o ./lib/libft/ft_strchr.o ./lib/libft/ft_strnstr.o  src/error_checker/error_checker.o \
src/error_checker/error_checker_helper.o ./lib/libft/ft_memcpy.o src/tokenizer/tokenizer.o \
./lib/libft/ft_strdup.o ./lib/libft/ft_strlcpy.o ./lib/libft/ft_substr.o ./lib/libft/ft_strncmp.o ./lib/libft/ft_isalnum.o ./lib/libft/ft_isdigit.o \
./lib/libft/ft_isalpha.o src/tokenizer/handle_double_quote.o src/tokenizer/token_operations.o src/tokenizer/tokenizer_helper.o ./lib/helper/helper.o

all : $(Name)

$(Name) : $(OBJ)
	$(CC) $(CFLAGS) -lreadline $^  -o $(Name)


%.o : %.c lib/libft/libft.h includes/error_checker.h includes/minishell.h includes/tokenizer.h includes/helper.h
	$(CC) $(CFLAGS) -c $< -o $@


clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(Name)

re : fclean all
