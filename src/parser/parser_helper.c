/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:43:34 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/04 13:01:49 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

void	free_redirections(t_redirection *redirs)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		free(current->file_or_delimiter);
		free(current);
		current = next;
	}
}

void	free_simple_cmd(t_simple_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->argc)
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	if (cmd->redirs)
		free_redirections(cmd->redirs);
	free(cmd);
}

void	free_table(t_cmd_table *cmd_table)
{
	int	i;

	if (!cmd_table)
		return ;
	i = 0;
	while (i < cmd_table->cmd_count)
	{
		free_simple_cmd(cmd_table->cmds[i]);
		i++;
	}
	free(cmd_table->cmds);
	free(cmd_table);
}
