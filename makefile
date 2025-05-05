Name = minishell
CC = cc
CFLAGS = -g #-Wall -Wextra -Werror
OBJ = main.o ./lib/libft/ft_strlen.o ./lib/libft/ft_strchr.o ./lib/libft/ft_strnstr.o  src/error_checker/error_checker.o  ./lib/helper/gc.o\
src/error_checker/error_checker_helper.o  src/executor/helper_exec/utils_exec.o  src/executor/executor.o src/executor/here_doc.o  src/executor/cmd/exec_cmd.o src/executor/cmd/check_cmd.o src/executor/cmd/utils_check_cmd.o  src/executor/load_env.o src/executor/shlvl.o src/executor/pwd_init.o  src/executor/builtins/echo.o \
src/executor/builtins/pwd.o src/executor/builtins/cd.o src/executor/builtins/env.o  src/executor/builtins/export.o src/executor/builtins/export2.o src/executor/builtins/export3.o src/executor/builtins/export4.o src/executor/builtins/export5.o src/executor/builtins/exit.o src/executor/builtins/unset.o src/executor/redirection/inf_outf.o  src/executor/redirection/utils_inf_outf.o src/executor/pipe/pipe.o src/executor/pipe/utils_pipe.o \
./lib/libft/ft_memcpy.o src/tokenizer/tokenizer.o ./lib/libft/ft_strdup.o ./lib/libft/ft_strlcpy.o ./lib/libft/ft_substr.o ./lib/libft/ft_strncmp.o ./lib/libft/ft_isalnum.o ./lib/libft/ft_isdigit.o ./lib/libft/ft_strtrim.o ./lib/libft/ft_split.o\
./lib/libft/ft_putstr_fd.o ./lib/libft/ft_atoi.o ./lib/libft/ft_itoa.o \
./lib/libft/ft_isalpha.o src/tokenizer/handle_double_quote.o src/tokenizer/token_operations.o src/tokenizer/tokenizer_helper.o ./lib/helper/helper.o \
src/parser/parser.o src/parser/parser_operations.o src/parser/parser_helper.o ./lib/libft/ft_strjoin.o ./lib/libft/ft_strlcat.o src/tokenizer/tokenizer_extra.o \
src/parser/parse_redir.o src/tokenizer/handle_var_expansion.o ./lib/helper/gc2.o

all : $(Name)

$(Name) : $(OBJ)
	$(CC) $(CFLAGS) $^  -o $(Name) -lreadline


%.o : %.c lib/libft/libft.h includes/error_checker.h includes/minishell.h includes/tokenizer.h includes/helper.h includes/parser.h includes/env.h includes/executor.h
	$(CC) $(CFLAGS) -c $< -o $@


clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(Name)

re : fclean all