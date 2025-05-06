/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:54 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/06 12:56:05 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"
#include <errno.h>

static void	error_exit(const char *msg, int code)
{
	perror(msg);
	exit(exit_stat(code, 1));
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

void	pars_cmd_1(char *cmd)
{
	if (cmd[0] == '.' && ft_check_path_cmd(cmd) == 0)
	{
		printf("minishell: %s: Command not found\n", cmd);
		exit(exit_stat(2, 1));
	}
}

void	pars_cmd_2(char **cmd, char **env)
{
	printf("cmd:%i\n", access(cmd[0], F_OK));
	if (access(cmd[0], F_OK) == -1)
	{
		printf("pass\n");
		error_exit(cmd[0], 127);
	}
	if (access(cmd[0], X_OK) == -1)
	{
		printf("minishell: %s: Permission Denied\n", cmd[0]);
		exit(exit_stat(126, 1));
	}
	execve(cmd[0], cmd, env);
	printf("minishell: %s: Command not found\n", cmd[0]);
	exit(exit_stat(127, 1));
}

void	pars_cmd_3(char **cmd, char **env)
{
	char	**path;
	char	*path_cmd;

	path = ft_split(fet_path(env), ':');
	if (!path || !path[0])
	{
		errno = ENOENT; 
		error_exit(cmd[0], 127);
	}
	path_cmd = ft_found_cmd(cmd[0], path);
	if (!path_cmd)
	{
		printf("minishell: %s: Command not found\n", cmd[0]);
		exit(exit_stat(127, 1));
	}
	execve(path_cmd, cmd, env);
	error_exit(cmd[0], 127);
}