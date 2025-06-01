/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:17:43 by achemlal          #+#    #+#             */
/*   Updated: 2025/06/01 10:08:45 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static int	check_too_many_args(t_simple_cmd *data)
{
	if (data->argc > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		exit_stat(1, 1, NULL, NULL);
		return (1);
	}
	return (0);
}

int	update_pwd(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*node;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, "OLDPWD") == 0)
			return (free(current->key), current->key = key,
				current->value = value, 0);
		else if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			free(key);
			current->value = value;
			return (0);
		}
		current = current->next;
	}
	node = create_env_node(key, value);
	add_node(env, node);
	return (0);
}

static char	*cd_get_target(t_simple_cmd *cmd, t_env *env)
{
	char	*target;

	target = NULL;
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

static char	*handle_getcwd_error(t_simple_cmd *data, t_env *env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*pwd;

	pwd = NULL;
	if (data->argc > 1 && ft_strcmp(data->args[1], "..") == 0)
	{
		if (chdir("..") != 0)
			exit_stat(1, 1, NULL, NULL);
		pwd = getcwd(NULL, 0);
		if (!pwd)
			write(2, "cd: error retrieving current directory: getcwd: cannot access \
					parent directories: No such file or directory\n", 113);
		free(pwd);
		oldpwd = ft_getenv(env, "PWD=");
		if (!oldpwd)
			return (NULL);
		newpwd = ft_strjoin(oldpwd, "/..");
		update_pwd(&env, ft_strdup("OLDPWD="), ft_strdup(oldpwd));
		update_pwd(&env, ft_strdup("PWD="), newpwd);
		return (NULL);
	}
	else
		return (ft_getenv(env, "PWD="));
}

void	builtin_cd(t_simple_cmd **data, t_env **env)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	target = NULL;
	if (check_too_many_args(*data))
		return ;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		oldpwd = handle_getcwd_error(*data, *env);
		if (!oldpwd)
			return ;
	}
	target = cd_get_target(*data, (*env));
	if (!target)
		return (free(oldpwd), (void)exit_stat(1, 1, NULL, NULL));
	if (chdir(target) != 0)
		return (write(2, "minishell: cd: ", 16), perror((*data)->args[1]),
			free(oldpwd), (void)exit_stat(1, 1, NULL, NULL));
	update_pwd(env, ft_strdup("OLDPWD="), ft_strdup(oldpwd));
	newpwd = getcwd(NULL, 0);
	if (newpwd)
		update_pwd(env, ft_strdup("PWD="), ft_strdup(newpwd));
	return (free(oldpwd), free(newpwd));
}
