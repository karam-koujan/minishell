/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:33 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/13 03:34:11 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>

typedef enum e_token_type
{
	PIPE_T,
	REDIR_F_T,
	REDIR_B_T,
	CMD_T,
	ARG_T,
	HERDOC_T,
	APPEND_T,
	VAR_T
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
void 	print_token_list(t_token *head);
void	ft_token_add_back(t_token **lst, t_token *new);

#endif