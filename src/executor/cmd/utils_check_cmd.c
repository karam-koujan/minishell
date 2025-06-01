/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:54 by achemlal          #+#    #+#             */
/*   Updated: 2025/06/01 17:39:25 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	error_exit(char *cmd, t_elem **elem, t_gc **gc)
{
	error_print(cmd, ": Command not found\n");
	exit(exit_stat(127, 1, gc, (*elem)->env));
}

void	exec_emp_path(char **cmd, t_elem **elem, t_gc **gc)
{
	if (access(cmd[0], F_OK) == 0)
	{
		if (access(cmd[0], X_OK) == -1)
		{
			error_print(cmd[0], ": Permission denied\n");
			exit(exit_stat(126, 1, gc, (*elem)->env));
		}
		else
		{
			if (execve(cmd[0], cmd, (*elem)->env) == -1)
				exit(exit_stat(0, 1, gc, (*elem)->env));
		}
	}
	else
	{
		error_print(cmd[0], ": No such file or directory\n");
		exit(exit_stat(127, 1, gc, (*elem)->env));
	}
}

void	pars_cmd_1(char *cmd, t_elem **elem, t_gc **gc)
{
	if (cmd == NULL || cmd[0] == '\0')
	{
		error_print(cmd, ": Command not found\n");
		exit(exit_stat(127, 1, gc, (*elem)->env));
	}
	if (!check_str(cmd, '/'))
	{
		error_print(cmd, ": Is a directory\n");
		exit(exit_stat(126, 1, gc, (*elem)->env));
	}
	if (cmd[0] == '.' && ft_strlen(cmd) == 1)
	{
		error_print(cmd, ": filename argument required\n");
		exit(exit_stat(2, 1, gc, (*elem)->env));
	}
}

void	pars_cmd_2(char **cmd, t_elem **elem, t_gc **gc)
{
	if (access(cmd[0], F_OK) == -1)
	{
		write(2, "minishell: ", 11);
		perror(cmd[0]);
		exit(exit_stat(127, 1, gc, (*elem)->env));
	}
	if (access(cmd[0], X_OK) == -1)
	{
		error_print(cmd[0], ": Permission Denied\n");
		exit(exit_stat(126, 1, gc, (*elem)->env));
	}
	if (execve(cmd[0], cmd, (*elem)->env) == -1)
	{
		if (access(cmd[0], F_OK | X_OK) == 0)
		{
			if (is_directory(cmd[0]))
			{
				error_print(cmd[0], ": Is a directory\n");
				exit(exit_stat(126, 1, gc, (*elem)->env));
			}
			exit(exit_stat(0, 1, gc, (*elem)->env));
		}
		error_print(cmd[0], ": Command not found\n");
		exit(exit_stat(127, 1, gc, (*elem)->env));
	}
}

void	pars_cmd_3(char **cmd, t_elem **elem, t_gc **gc)
{
	char	**path;
	char	*path_cmd;

	path = ft_split(fet_path((*elem)->env), ':');
	if (!path || !path[0])
		exec_emp_path(cmd, elem, gc);
	path_cmd = ft_found_cmd(cmd[0], path);
	free_arr(path);
	if (!path_cmd)
	{
		error_print(cmd[0], ": Command not found\n");
		exit(exit_stat(127, 1, gc, (*elem)->env));
	}
	execve(path_cmd, cmd, (*elem)->env);
	if (access(path_cmd, F_OK) == 0)
	{
		if (access(path_cmd, X_OK) == -1)
		{
			error_print(path_cmd, ": Permission Denied\n");
			exit(exit_stat(126, 1, gc, (*elem)->env));
		}
		exit(exit_stat(0, 1, gc, (*elem)->env));
	}
	free(path_cmd);
	error_exit(cmd[0], elem, gc);
}
