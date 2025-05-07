/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:33 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/07 11:44:21 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "../lib/libft/libft.h"
# include "../includes/helper.h"
# include "../includes/env.h"
# include <stdlib.h>
# include <stdio.h>

typedef enum e_token_type
{
	PIPE_T,
	REDIR_F_T,
	REDIR_B_T,
	WORD_T,
	HERDOC_T,
	APPEND_T,
	VAR_T,
	SP_T,
	QT_T,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*val;
	int				v_in_qt;
	struct s_token	*next;
}				t_token;

typedef struct s_token_data
{
	t_token_type	type;
	int				v_in_qt;
}				t_token_data;

typedef struct s_double_quote
{
	char	*start;
	int		len;
}				t_double_quote;

char	*quote_token(char **cmd, char end_char);
t_token	*init_token(t_token_type type, char *val, int vr_in_qt);
t_token	*tokenize(char *cmd, t_env *env);
void	print_token_list(t_token *head);
void	ft_token_add_back(t_token **lst, t_token *new);
int		handle_double_quote(char *cmd, t_token **head, int *in_herdoc);
int		loop_double_quote(char **cmd_ptr, t_token **head, \
t_double_quote *st, int *in_herdoc);
int		double_quote_len(char *cmd);
void	ft_token_add_back(t_token **lst, t_token *new);
int		add_token(t_token **head, char *start_ptr, int len, t_token_type type);
int		is_var_spchar(char c);
int		handle_var(char *cmd, t_token **head, int in_quote);
void	ft_token_lstclear(t_token **lst, void (*del)(void*));
int		handle_whitespace(char *cmd, t_token **head, int *in_herdoc);
int		handle_word(char *cmd, t_token **head, int *in_herdoc);
char	*remove_quotes(char *cmd);
t_token	*handle_tokenizer(t_token **tokenlst, t_env *env);
void	join_var(t_token **tokenlst, t_env *env);
void	join_cmd(t_token **tokenlst);
void	handle_word_token(t_token *lst, t_token **next_ptr);
int	add_vr_token(t_token **head, char *start_ptr, int len, \
	t_token_data *token_data);
#endif
