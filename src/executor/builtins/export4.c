/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:17 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/31 15:37:56 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static int	check_key_only(t_env *env, char *key)
{
	char	*str;

	while (env)
	{
		str = ft_strtrim(env->key, "=");
		if (!str)
			return (0);
		if (ft_strcmp(str, key) == 0)
		{
			free(str);
			free(key);
			return (1);
		}
		free(str);
		env = env->next;
	}
	return (0);
}

static int	update_existing(t_env *env, t_elem **elem)
{
	char	*str;
	char	*new;

	while (env)
	{
		str = ft_strtrim(env->key, "=");
		new = ft_strtrim((*elem)->key, "=");
		if (!str || !new)
			return (free(str), free(new), 0);
		if (ft_strcmp(str, new) == 0)
			return (free(str), free(new), free(env->key), \
			free(env->value), env->key = (*elem)->key,
				env->value = (*elem)->value, 1);
		else
		{
			free(str);
			free(new);
			env = env->next;
		}
	}
	return (0);
}

static int	update_var(t_env **env, t_elem **elem)
{
	if (ft_strchr((*elem)->key, '+'))
	{
		handle_plus(env, &(*elem)->key, &(*elem)->value);
		return (1);
	}
	if (!ft_strchr((*elem)->key, '='))
		return (check_key_only(*env, (*elem)->key));
	return (update_existing(*env, elem));
}

static void	add_var(t_env **env, t_elem **elem)
{
	t_env	*new_node;

	if (update_var(env, elem) == 1)
		return ;
	if (ft_strchr((*elem)->key, '+'))
		handle_plus(env, &(*elem)->key, &(*elem)->value);
	new_node = create_env_node((*elem)->key, (*elem)->value);
	add_node(env, new_node);
}

void	var_set(char *str, t_env **env, t_elem **elem)
{
	size_t	size;

	(*elem)->key = NULL;
	(*elem)->value = NULL;
	size = 0;
	if (ft_strchr(str, '='))
	{
		size = ft_strchr(str, '=') - str + 1;
		(*elem)->key = ft_substr(str, 0, size);
		(*elem)->value = ft_substr(str, size, ft_strlen(str) - size);
		if (!(*elem)->key || !(*elem)->value)
			return (free((*elem)->key), free((*elem)->value));
	}
	else
	{
		(*elem)->key = ft_strdup(str);
		if (!(*elem)->key)
			return ;
	}
	add_var(env, elem);
}
