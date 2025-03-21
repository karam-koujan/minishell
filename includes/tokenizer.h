/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:33 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/21 22:44:46 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "../lib/libft/libft.h"
# include "../includes/helper.h"
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
	SP_T
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*val;
	struct s_token	*next;
}			t_token;

char	*quote_token(char **cmd, char end_char);
t_token	*init_token(t_token_type type, char *val);
t_token	*tokenize(char *cmd);
void	print_token_list(t_token *head);
void	ft_token_add_back(t_token **lst, t_token *new);
int		handle_double_quote(char *cmd, t_token **head);
int		loop_double_quote(char **cmd_ptr, t_token **head, \
char **start, int *len);
int		double_quote_len(char *cmd);
t_token	*init_token(t_token_type type, char *val);
void	ft_token_add_back(t_token **lst, t_token *new);
int		add_token(t_token **head, char *start_ptr, int len, t_token_type type);
int		is_var_spchar(char c);
int		handle_var(char *cmd, t_token **head);
void	ft_token_lstclear(t_token **lst, void (*del)(void*));
#endif
