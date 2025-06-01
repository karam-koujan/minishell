/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inf_outf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:37:46 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/31 16:17:11 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	handle_redir_in_child(t_simple_cmd *cmd, t_elem **elem, t_gc **gc)
{
	t_redirection	*files;

	files = cmd->redirs;
	while (files)
	{
		if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
			in_cas(files, elem, gc);
		else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
			ou_cas(files, elem, gc);
		files = files->next;
	}
}

int	check_redir_in_parent(t_simple_cmd *cmd)
{
	t_redirection	*files;

	files = cmd->redirs;
	while (files)
	{
		if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
		{
			if (in_cas_p(files) == 0)
				return (exit_stat(1, 1, NULL, NULL), 0);
		}
		else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
		{
			if (ou_cas_p(files) == 0)
				return (exit_stat(1, 1, NULL, NULL), 0);
		}
		files = files->next;
	}
	return (1);
}

void	inf_outf_cmd(t_simple_cmd **data, int flag, t_elem **elem, t_gc **gc)
{
	if (flag == 1)
		handle_redir_in_child(*data, elem, gc);
	else
		check_redir_in_parent(*data);
}
