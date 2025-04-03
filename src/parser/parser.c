/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 03:09:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/03 13:00:15 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

void	add_cmd_to_table(t_cmd_table *cmd_table, t_simple_cmd *cmd)
{
	int				cmd_c;
	t_simple_cmd	**new_cmds;
	int				i;

	i = -1;
	cmd_c = cmd_table->cmd_count;
	new_cmds = malloc((cmd_c + 2) * sizeof(t_simple_cmd *));
	if (!new_cmds)
		return ;
	while (++i < cmd_c)
		new_cmds[i] = cmd_table->cmds[i];
	new_cmds[i] = cmd;
	new_cmds[++i] = NULL;
	cmd_table->cmds = new_cmds;
	cmd_table->cmd_count = cmd_c + 1;
}

t_token	*parse_word(t_simple_cmd **cmd, t_token *token)
{
	if (*cmd == NULL)
		*cmd = create_simple_cmd();
	if (*cmd == NULL)
		return (NULL);
	add_arg_to_cmd(*cmd, token->val);
	return (token);
}

t_token	*parse_redir(t_simple_cmd **cmd, t_token *token)
{
	t_redirection	*redir;
	t_redir_type	redir_type;

	if (token->type == REDIR_B_T)
		redir_type = REDIR_IN;
	else if (token->type == REDIR_F_T)
		redir_type = REDIR_OUT;
	else if (token->type == APPEND_T)
		redir_type = REDIR_APPEND;
	else
		redir_type = REDIR_HEREDOC;
	while (token && token->type != WORD_T)
		token = token->next;
	redir = create_redirection(redir_type, token->val);
	if (redir == NULL || cmd == NULL)
		return (NULL);
	add_redir_to_cmd(*cmd, redir);
	return (token);
}

t_cmd_table	*parse(t_token *tokenlst)
{
	t_token			*token;
	t_cmd_table		*cmd_table;
	t_simple_cmd	*simple_cmd;

	token = tokenlst;
	cmd_table = create_command_table();
	simple_cmd = NULL;
	while (token)
	{
		if (token->type == WORD_T)
		{
			token = parse_word(&simple_cmd, token);
			if (!token)
				return (NULL);
		}
		else if (token->type == APPEND_T || token->type == HERDOC_T \
			|| token->type == REDIR_B_T || token->type == REDIR_F_T)
		{
			token = parse_redir(&simple_cmd, token);
			if (!token)
				return (NULL);
		}
		else if (token->type == PIPE_T)
		{
			add_cmd_to_table(cmd_table, simple_cmd);
			simple_cmd = NULL;
		}
		token = token->next;
	}
	if (simple_cmd != NULL)
		add_cmd_to_table(cmd_table, simple_cmd);
	return (cmd_table);
}
