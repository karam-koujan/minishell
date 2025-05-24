/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:23:22 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 13:11:45 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_key_exists(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

int	add_pwd_if_missing(t_env **env)
{
	t_env	*new_node;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (0);
	new_node = create_env_node(ft_strdup("PWD="), pwd);
	if (!new_node)
		return (free(pwd), 0);
	add_node(env, new_node);
	return (1);
}

int	add_oldpwd_if_missing(t_env **env)
{
	t_env	*new_node;

	new_node = create_env_node(ft_strdup("OLDPWD"), NULL);
	if (!new_node)
		return (0);
	add_node(env, new_node);
	return (1);
}

void	init_pwd(t_env **env, t_gc **gc)
{
	if (env_key_exists(*env, "PWD="))
		return ;
	if (!add_pwd_if_missing(env))
		return ;
	if (!env_key_exists(*env, "OLDPWD"))
	{
		if (!add_oldpwd_if_missing(env))
			return ;
	}
}
