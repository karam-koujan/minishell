/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:31:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/22 14:43:11 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

t_redirection	*redir_file(t_token **token, t_env *env, t_redir_type type)
{
	t_redirection	*redir;
	char			*val;
	int				in_quote;

	in_quote = 0;
	while (*token && ((*token)->type != WORD_T && (*token)->type != VAR_T))
	{
		if ((*token)->type == QT_T)
			in_quote = 1;
		*token = (*token)->next;
	}
	val = join_expnd(*token, env);
	if (*token && (*token)->type == WORD_T)
		redir = create_redirection(type, val, 0);
	else if (*token && (*token)->type == VAR_T)
		redir = create_redirection(type, val, !in_quote);
	free(val);
	return (redir);
}

t_token	*parse_redir(t_simple_cmd **cmd, t_token *token, t_env *env)
{
	t_redirection	*redir;
	t_redir_type	redir_type;
	char			*val;

	if (token->type == REDIR_B_T)
		redir_type = REDIR_IN;
	else if (token->type == REDIR_F_T)
		redir_type = REDIR_OUT;
	else if (token->type == APPEND_T)
		redir_type = REDIR_APPEND;
	else
		redir_type = REDIR_HEREDOC;
	redir = redir_file(&token, env, redir_type);
	if (redir == NULL || cmd == NULL)
		return (NULL);
	if (*cmd == NULL)
		*cmd = create_simple_cmd();
	add_redir_to_cmd(*cmd, redir);
	return (token);
}
