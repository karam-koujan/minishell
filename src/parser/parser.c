/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 03:09:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/21 09:36:02 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../includes/executor.h"

void	add_cmd_to_table(t_cmd_table *cmd_table, t_simple_cmd *cmd)
{
	int				cmd_c;
	t_simple_cmd	**new_cmds;
	int				i;

	i = -1;
	if (!cmd_table || !cmd)
		return ;
	cmd_c = cmd_table->cmd_count;
	new_cmds = malloc((cmd_c + 2) * sizeof(t_simple_cmd *));
	if (!new_cmds)
		return ;
	while (++i < cmd_c)
		new_cmds[i] = cmd_table->cmds[i];
	new_cmds[i] = cmd;
	new_cmds[++i] = NULL;
	free(cmd_table->cmds);
	cmd_table->cmds = new_cmds;
	cmd_table->cmd_count = cmd_c + 1;
}

char	*ft_getenv_val(t_env *env, char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->key) - 1 == ft_strlen(key) && \
		ft_strncmp(env->key, key, ft_strlen(env->key) - 1) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*join_expnd(t_token *token, t_env *env)
{
	char	*value;
	char	*prev;
	char	*tmp;

	tmp = NULL;
	prev = NULL;
	value = token->val;
	while (token && (token->type == WORD_T || token->type == VAR_T))
	{
		if (token->type == VAR_T)
		{
			value = ft_getenv_val(env, token->val);
			tmp = value;
		}
		else
			value = token->val;
		value = ft_strjoin(prev, value);
		free(tmp);
		tmp = NULL;
		if (value == NULL)
			return (free(prev), NULL);
		free(prev);
		prev = value;
		token = token->next;
	}
	return (value);
}

t_token	*parse_word(t_simple_cmd **cmd, t_token *token, t_env *env)
{
	char	*value;
	char	*prev;

	prev = NULL;
	if (*cmd == NULL)
		*cmd = create_simple_cmd();
	if (*cmd == NULL)
		return (NULL);
	value = join_expnd(token, env);
	while (token && (token->type == WORD_T || token->type == VAR_T))
		token = token->next;
	add_arg_to_cmd(*cmd, value);
	free(value);
	return (token);
}

t_token	*parse_redir(t_simple_cmd **cmd, t_token *token, t_env *env)
{
	t_redirection	*redir;
	t_redir_type	redir_type;
	char			*val;
	int				in_quote;

	in_quote = 0;
	if (token->type == REDIR_B_T)
		redir_type = REDIR_IN;
	else if (token->type == REDIR_F_T)
		redir_type = REDIR_OUT;
	else if (token->type == APPEND_T)
		redir_type = REDIR_APPEND;
	else
		redir_type = REDIR_HEREDOC;
	while (token && (token->type != WORD_T && token->type != VAR_T))
	{
		if (token->type == QT_T)
			in_quote = 1;
		token = token->next;
	}
	val = join_expnd(token, env);
	if (token && token->type == WORD_T)
		redir = create_redirection(redir_type, val, 0);
	else if (token && token->type == VAR_T)
		redir = create_redirection(redir_type, val, !in_quote);
	free(val);
	if (redir == NULL || cmd == NULL)
		return (NULL);
	if (*cmd == NULL)
		*cmd = create_simple_cmd();
	add_redir_to_cmd(*cmd, redir);
	return (token);
}

t_token	*parse_token(t_cmd_table **cmd_table, \
	t_simple_cmd **simple_cmd, t_token *token, t_env *env)
{
	if (token->type == WORD_T)
	{
		token = parse_word(simple_cmd, token, env);
		if (!token)
			return (NULL);
	}
	if (token->type == VAR_T)
	{
		token = parse_word(simple_cmd, token, env);
		if (!token)
			return (NULL);
	}
	else if (token->type == APPEND_T || token->type == HERDOC_T \
		|| token->type == REDIR_B_T || token->type == REDIR_F_T)
	{
		token = parse_redir(simple_cmd, token, env);
		if (!token)
			return (NULL);
	}
	else if (token->type == PIPE_T)
	{
		add_cmd_to_table(*cmd_table, *simple_cmd);
		*simple_cmd = NULL;
	}
	return (token->next);
}

t_cmd_table	*parse(t_token *tokenlst, t_env *env)
{
	t_token			*token;
	t_cmd_table		*cmd_table;
	t_simple_cmd	*simple_cmd;

	token = tokenlst;
	cmd_table = create_command_table();
	if (!cmd_table)
		return (NULL);
	simple_cmd = NULL;
	while (token)
		token = parse_token(&cmd_table, &simple_cmd, token, env);
	if (simple_cmd != NULL)
		add_cmd_to_table(cmd_table, simple_cmd);
	return (cmd_table);
}
