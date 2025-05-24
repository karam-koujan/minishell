/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:45 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 16:14:43 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	check_is_building(t_simple_cmd **data, t_env *env, t_elem **elem,
			t_gc **gc)
{
	if ((*data)->argc == 0)
		exit(exit_stat(1, 1, gc, (*elem)->env));
	else if (ft_strcmp((*data)->args[0], "echo") == 0)
		return (builtin_echo(data), 1);
	else if (ft_strcmp((*data)->args[0], "pwd") == 0)
		return (builtin_pwd(data), 1);
	else if (ft_strcmp((*data)->args[0], "env") == 0)
		return (builtin_env(data, env), 1);
	else if (ft_strcmp((*data)->args[0], "export") == 0)
		return (builtin_export_child(data, &env, gc, elem), 1);
	else if (ft_strcmp((*data)->args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp((*data)->args[0], "exit") == 0)
		return (builtin_exit_child(data, gc, elem), 1);
	else if (ft_strcmp((*data)->args[0], "cd") == 0)
		return (builtin_cd_child(data, &env, gc, elem), 1);
	return (0);
}

void	exec_proc(t_simple_cmd **data, t_env *env, t_elem **elem, t_gc **gc)
{
	inf_outf_cmd(data, 1, elem, gc);
	if (check_is_building(data, env, elem, gc))
		exit(exit_stat(0, 1, gc, (*elem)->env));
	check_exec_cmd((*data)->args, elem, gc);
}

void	exec_cmd(t_simple_cmd **data, t_env *env, t_elem **elem, t_gc **gc)
{
	pid_t	pid;
	int		status;

	if (!(*data)->args || !(*data)->args[0])
		return ;
	pid = fork();
	if (pid == -1)
		return (perror("Error: fork"));
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		exec_proc(data, env, elem, gc);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (!WIFSIGNALED(status))
			exit_status(status);
		else if (exit_stat(0, 0, NULL, NULL) == 130)
			printf("\n");
		else if (status == 131 && printf("\n"))
			exit_stat(status, 1, NULL, NULL);
	}
}
