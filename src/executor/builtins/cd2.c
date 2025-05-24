/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:21:49 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 14:46:50 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static char	*cd_get_target_child(t_simple_cmd *cmd, t_env *env, t_gc **gc)
{
	char	*target;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "--") == 0)
	{
		target = ft_getenv(env, "HOME=");
		if (!target)
			return (write(2, "minishell: cd: HOME not set\n", 29), NULL);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		target = ft_getenv(env, "OLDPWD=");
		if (!target)
			return (write(2, "minishell: cd: OLDPWD not set\n", 31), NULL);
		else
			return (printf("%s\n", target), target);
	}
	else
		target = cmd->args[1];
	return (target);
}

void	builtin_cd_child(t_simple_cmd **data, t_env **env, t_gc **gc,
		t_elem **elem)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	target = NULL;
	if ((*data)->argc > 2)
		return (write(2, "minishell: cd: too many arguments\n", 35),
			(void)exit(exit_stat(1, 1, NULL, NULL)));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (write(2, "minishell: cd: getcwd failed", 29),
			(void)exit(exit_stat(1, 1, gc, (*elem)->env)));
	target = cd_get_target_child(*data, (*env), gc);
	if (!target)
		return (free(oldpwd), (void)exit(exit_stat(1, 1, gc, (*elem)->env)));
	if (chdir(target) != 0)
	{
		write(2, "minishell: cd: ", 16);
		write(2, target, ft_strlen(target));
		write(2, ": No such file or directory\n", 29);
		return (free(oldpwd), (void)exit(exit_stat(1, 1, NULL, NULL)));
	}
}
