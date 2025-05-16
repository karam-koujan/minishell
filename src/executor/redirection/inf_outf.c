/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inf_outf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:37:46 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/15 19:24:42 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../../includes/minishell.h"


void handle_redir_in_child(t_simple_cmd *cmd, t_elem **elem, t_gc **gc)
{
	t_redirection *files = cmd->redirs;

	while (files)
	{
		if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
			in_cas(files, elem, gc);
		else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
			ou_cas(files, elem, gc);
		files = files->next;
	}
}

void check_redir_in_parent(t_simple_cmd *cmd,  t_elem **elem, t_gc **gc)
{
	t_redirection *files = cmd->redirs;

	while (files)
	{
		if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
		{
			if (in_cas_p(files, elem, gc) == 0)
				exit_stat(1, 1, NULL, NULL);
		}
		else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
		{
			if (ou_cas_p(files, elem, gc) == 0)
				exit_stat(1, 1, NULL, (*elem)->env);
		}
		files = files->next;
	}
}
void inf_outf_cmd(t_simple_cmd **data, int flag, t_elem **elem, t_gc **gc)
{
	if (flag == 1)
		return handle_redir_in_child(*data, elem, gc);
	else
		return check_redir_in_parent(*data, elem, gc);
}
