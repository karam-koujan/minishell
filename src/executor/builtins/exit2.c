/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 14:56:59 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/31 15:52:41 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

void	free_exit(t_gc **gc, char **env)
{
	if (gc)
		free_all(gc);
	if (env)
		free_arr(env);
}

int	exit_stat(int value, int action, t_gc **gc, char **env)
{
	static int	status = 0;

	free_exit(gc, env);
	if (action == 1)
		status = value;
	return (status);
}

void	exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		exit_stat(128 + WTERMSIG(status), 1, NULL, NULL);
	}
	else if (WIFEXITED(status) == 1)
		exit_stat(WEXITSTATUS(status), 1, NULL, NULL);
}
