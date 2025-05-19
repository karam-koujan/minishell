/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:54 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/18 18:48:42 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"
#include <errno.h>

static void	error_exit(const char *msg, int code, char **env, t_gc **gc)
{
	write(1, "minishell: ", 12);
	perror(msg);
	exit(exit_stat(code, 1, NULL, env));
}
int	ft_check_path_cmd(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	pars_cmd_1(char *cmd, t_elem **elem , t_gc **gc)
{
	int i ;
	int flag;
	
	i = 0;
	flag = 0;
	if(cmd[0] == '/')
	{
		while(cmd[i])
		{
			if(cmd[i] != '/')
				flag = 1;
			i++;
		}
		if(flag == 0)
			printf("minishell: %s:  Is a directory\n", cmd), exit(exit_stat(126, 1, gc, (*elem)->env));
	}
	if (cmd[0] == '.' && ft_check_path_cmd(cmd) == 0)
	{
		printf("minishell: %s: Command not found\n", cmd);
		exit(exit_stat(2, 1, gc, (*elem)->env));
	}
}

void	pars_cmd_2(char **cmd, t_elem **elem, t_gc **gc)
{
	char	*str;
	char	*tmp;

	if (access(cmd[0], F_OK) == -1)
	{

		error_exit(cmd[0], 127, (*elem)->env, gc);
	}
	if (access(cmd[0], X_OK) == -1)
	{
		printf("minishell: %s: Permission Denied\n", cmd[0]);
		return (exit(exit_stat(126, 1, gc, (*elem)->env)));
	}
	if (execve(cmd[0], cmd, (*elem)->env) == -1)
	{
		if(access(cmd[0], F_OK | X_OK) == 0)
			exit(exit_stat(0, 1, gc, (*elem)->env));
		printf("minishell: %s: Command not found\n", cmd[0]);
	}
	return (exit(exit_stat(127, 1, gc, (*elem)->env)));
}

void	pars_cmd_3(char **cmd, t_elem **elem, t_gc **gc)
{
	char	**path;
	char	*path_cmd;

	path = ft_split(fet_path((*elem)->env), ':');
	if (!path || !path[0])
	{
		errno = ENOENT; 
		free_arr(path);
		error_exit(cmd[0], 127, (*elem)->env, gc);
	}
	path_cmd = ft_found_cmd(cmd[0], path);
	free_arr(path);
	if (!path_cmd)
	{
		printf("minishell: %s: Command not found\n", cmd[0]);
		exit(exit_stat(127, 1, gc, (*elem)->env));
	}
	execve(path_cmd, cmd, (*elem)->env);
	free(path_cmd);
	error_exit(cmd[0], 127, (*elem)->env, gc);
}
