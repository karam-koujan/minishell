/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inf_outf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:37:46 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/20 16:56:01 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../../includes/minishell.h"


int handle_redir_in_child(t_simple_cmd *cmd)
{
	t_redirection *files = cmd->redirs;

	while (files)
	{
		if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
			in_cas(files);
		else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
			ou_cas(files);
		files = files->next;
	}
	return (0);
}

int check_redir_in_parent(t_simple_cmd *cmd)
{
	t_redirection *files = cmd->redirs;

	while (files)
	{
		if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
		{
			if (in_cas_p(files) == 0)
				return (exit_stat(1, 1), 1);
		}
		else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
		{
			if (ou_cas_p(files) == 0)
				return (exit_stat(1, 1), 1);
		}
		files = files->next;
	}
	return (0);
}
int inf_outf_cmd(t_simple_cmd **data, int flag)
{
	if (flag == 1)
		return handle_redir_in_child(*data);
	else
		return check_redir_in_parent(*data);
}
