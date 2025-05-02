/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:41 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/19 18:22:41 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"

char	*fet_path(char **env)
{
	int		i;
	char	*path;

	i = 0;
	path = "PATH=";
	while (env[i])
	{
		if (ft_strncmp(env[i], path, 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*ft_found_cmd(char *cmd, char **path)
{
	int		i;
	char	*cmd_path_join;
	char	*cmd_path_join_2;

	i = 0;
	while (path[i])
	{
		cmd_path_join = ft_strjoin(path[i], "/");
		cmd_path_join_2 = ft_strjoin(cmd_path_join, cmd);
		free(cmd_path_join);
		if (access(cmd_path_join_2, X_OK) == 0)
			return (cmd_path_join_2);
		free(cmd_path_join_2);
		i++;
	}
	return (NULL);
}

void	check_exec_cmd(char **cmd, char **env)
{
	pars_cmd_1(cmd[0]);
	if (ft_check_path_cmd(cmd[0]) == 1)
		pars_cmd_2(cmd, env);
	pars_cmd_3(cmd, env);
}