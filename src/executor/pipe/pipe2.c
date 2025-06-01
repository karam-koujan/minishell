/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:02:49 by achemlal          #+#    #+#             */
/*   Updated: 2025/06/01 10:04:09 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	handle_pipe_signal(int status)
{
	if (exit_stat(0, 0, NULL, NULL) == 0 || exit_stat(0, 0, NULL, NULL) == 127)
		g_gl = 2;
	if (exit_stat(0, 0, NULL, NULL) == 130 || status == 131)
		write(1, "\n", 1);
}

int	is_redir_exist(t_redirection *redirs, t_redir_type type)
{
	t_redirection	*r;

	if (!redirs)
		return (-1);
	r = redirs;
	while (r)
	{
		if (r->type == type)
			return (1);
		r = r->next;
	}
	return (0);
}
