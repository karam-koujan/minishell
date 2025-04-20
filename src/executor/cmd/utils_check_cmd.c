/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:54 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/19 19:21:29 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"

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

int	pars_cmd_1(char *cmd)
{

	if ( cmd[0] == '.' && ft_check_path_cmd(cmd) == 0)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Command not found\n", 2);
		return (-1);
	}
	return (0);
}

int	pars_cmd_2(char **cmd, char **env)
{
	if (access(cmd[0], F_OK) == -1)
	{
		perror(cmd[0]);
		return (-1);
	}
	if (access(cmd[0], X_OK) == -1)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": Permission Denied\n", 2);//exit status 126 
		return (-1);
	}
	if (execve(cmd[0], cmd, env) == -1)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": Command not found\n", 2);
		return (-1);
	}
	return (0);
}

int	pars_cmd_3(char **cmd, char **env)
{
	char	**path;
	char	*path_cmd;

	path = ft_split(fet_path(env), ':');
	if (!path)
		return (ft_putstr_fd("Error\n", 2), -1);
	path_cmd = ft_found_cmd(cmd[0], path);
	if (!path_cmd)
	{
		ft_putstr_fd(cmd[0], 1);
		ft_putstr_fd(": Command not found\n", 2);
	}
	if (execve(path_cmd, cmd, env) == -1)
		return (ft_putstr_fd("Execve Failed\n", 2), -1);
	return (0);
}
