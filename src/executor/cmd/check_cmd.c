/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:41 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/22 15:39:13 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"

char	*fet_path(char **env)
{
	int		i;
	char	*path;

	i = 0;
	path = "PATH=";
	if(!env)
		return NULL;
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

void	check_exec_cmd(char **cmd, t_elem **elem, t_gc **gc)
{
	pars_cmd_1(cmd[0], elem, gc);
	if (ft_check_path_cmd(cmd[0]) == 1)
		pars_cmd_2(cmd, elem, gc);
	pars_cmd_3(cmd, elem, gc);
}