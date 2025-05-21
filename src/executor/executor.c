/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:16:34 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/21 17:40:31 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void exec_builtin(t_simple_cmd **data, t_env **env, t_elem **elem, t_gc **gc)
{
	if (!ft_strcmp((*data)->args[0], "cd"))
		builtin_cd(data, env, gc, elem);
	else if (!ft_strcmp((*data)->args[0], "exit"))
		builtin_exit(data, gc, elem);
	else if (!ft_strcmp((*data)->args[0], "unset"))
		builtin_unset(data, env, gc);
}

int is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") 
		|| !ft_strcmp(cmd, "unset"));
}

void single_cmd(t_simple_cmd **data, t_env **env, t_elem *elem, t_gc **gc)
{
	if ((*data)->argc == 0)
		inf_outf_cmd(data, 0, &elem, gc);
	else if (is_builtin((*data)->args[0])) 
		exec_builtin(data, env, &elem, gc);
	else
		exec_cmd(data, *env, &elem, gc);
}



void exec(t_cmd_table *data, t_env **env, t_gc **gc)
{
	t_elem elem;

	if (!data || !env || !(*env))
		return; 

	elem.env = env_list_to_array(*env);
	if (!elem.env)
		return; 

	handle_herdoc(&data, &elem, gc);
	if (data->cmd_count == 1)
	{
		g_gl = 1;
		single_cmd(data->cmds, env, &elem, gc);
		close_single_fd(data->cmds[0]);
	}
	else if (data->cmd_count > 1)
	{
		g_gl = 1;
		pipe_case(data, *env, &elem, gc);
		close_pipe_fd(&data);
	}
	free_arr(elem.env);
}
