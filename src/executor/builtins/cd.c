/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:17:43 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/28 20:44:42 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

void	update_pwd(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*node;

	if (!env || !*env || !key || !value)
		return ;
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			free(key);
			current->value = value;
			if (!current->value)
				return ;
			return ;
		}
		current = current->next;
	}
	node = create_env_node(key, value);
	add_node(env, node);
}

static char	*cd_get_target(t_simple_cmd *cmd, t_env *env, t_gc **gc)
{
	char	*target;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "--") == 0)
	{
		target = ft_getenv(env, "HOME=");
		if (!target)
			return (printf("minishell: cd: HOME not set\n"), NULL);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		target = ft_getenv(env, "OLDPWD=");
		if (!target)
			return (printf("minishell: cd: OLDPWD not set\n"), NULL);
		else
			return (printf("%s\n", target), target);
	}
	else
		target = cmd->args[1];
	return (target);
}

void	builtin_cd(t_simple_cmd **data, t_env **env, t_gc **gc, t_elem **elem)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	target = NULL;
	if ((*data)->argc > 2)
		return (printf("minishell: cd: too many arguments\n"),
			(void)exit_stat(1, 1, NULL, NULL));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		if ((*data)->argc > 1 && ft_strcmp((*data)->args[1], "..") == 0)
		{
			if (chdir("..") != 0 || ft_strcmp((*data)->args[1], ".."))
					exit_stat(1, 1, NULL, NULL);
			if(!getcwd(NULL, 0))
				write(2, "cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 108);
			oldpwd = ft_getenv(*env, "PWD=");
			if(!oldpwd)
				return ;
			newpwd = ft_strjoin(oldpwd, "/..");
			update_pwd(env, ft_strdup("OLDPWD="), ft_strdup(oldpwd));
			update_pwd(env, ft_strdup("PWD="), newpwd); 
			return ;
		}
		else
		{
			oldpwd = ft_getenv(*env, "PWD=");
		}
	}
	target = cd_get_target(*data, (*env), gc);
	if (!target)
		return (free(oldpwd), (void)exit_stat(1, 1, NULL, NULL));
	if (chdir(target) != 0)
		return (write(2, "minishell: cd: ", 16), perror((*data)->args[1]), free(oldpwd),
			(void)exit_stat(1, 1, NULL, NULL));
	update_pwd(env, ft_strdup("OLDPWD="), ft_strdup(oldpwd));
	newpwd = getcwd(NULL, 0);
	if (newpwd)
		update_pwd(env, ft_strdup("PWD="), ft_strdup(newpwd));
	free(oldpwd);
	free(newpwd);
}
