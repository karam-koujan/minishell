/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:31:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/03 12:56:13 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

t_redirection	*create_redirection(t_redir_type type, char *file_or_delimiter)
{
	t_redirection	*result;

	result = malloc(sizeof(t_redirection));
	if (!result)
		return (NULL);
	result->type = type;
	result->file_or_delimiter = ft_strdup(file_or_delimiter);
	if (!result->file_or_delimiter)
		return (NULL);
	result->next = NULL;
	return (result);
}

t_simple_cmd	*create_simple_cmd(void)
{
	t_simple_cmd	*result;

	result = malloc(sizeof(t_simple_cmd));
	if (!result)
		return (NULL);
	result->args = NULL;
	result->argc = 0;
	result->redirs = NULL;
	return (result);
}

t_cmd_table	*create_command_table(void)
{
	t_cmd_table	*result;

	result = malloc(sizeof(t_cmd_table));
	if (!result)
		return (NULL);
	result->cmds = NULL;
	result->cmd_count = 0;
	return (result);
}

void	add_redir_to_cmd(t_simple_cmd *cmd, t_redirection *redir)
{
	t_redirection	*redirs;

	if (!cmd || !redir)
		return ;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		redirs = cmd->redirs;
		while (redirs != NULL && redirs->next != NULL)
			redirs = redirs->next;
		redirs->next = redir;
	}
}

void	add_arg_to_cmd(t_simple_cmd *cmd, char *arg)
{
	int		arg_c;
	char	**new_arg;
	int		i;

	i = -1;
	arg_c = cmd->argc;
	new_arg = malloc((arg_c + 2) * sizeof(char *));
	if (!new_arg)
		return ;
	while (++i < arg_c)
		new_arg[i] = cmd->args[i];
	new_arg[i] = arg;
	new_arg[++i] = NULL;
	cmd->args = new_arg;
	cmd->argc = arg_c + 1;
}

