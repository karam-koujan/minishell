/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:17 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 18:46:30 by achemlal         ###   ########.fr       */
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
			return (1);
		}
		free(str);
		env = env->next;
	}
	return (0);
}

static int	update_existing(t_env *env, char *key, char *value)
{
	char	*str;
	char	*new;

	while (env)
	{
		str = ft_strtrim(env->key, "=");
		new = ft_strtrim(key, "=");
		if (!str || !new)
		{
			free(str);
			free(new);
			return (0);
		}
		if (ft_strcmp(str, new) == 0)
		{
			free(str);
			free(new);
			env->key = key;
			env->value = value;
			return (1);
		}
		free(str);
		free(new);
		env = env->next;
	}
	return (0);
}

int	update_var(t_env **env, char *key, char *value, t_gc **gc)
{
	if (ft_strchr(key, '+'))
	{
		handle_plus(env, &key, &value, gc);
		return (1);
	}
	if (!ft_strchr(key, '='))
		return (check_key_only(*env, key));
	return (update_existing(*env, key, value));
}

void	add_var(t_env **env, char *key, char *value, t_gc **gc)
{
	t_env	*new_node;

	if (update_var(env, key, value, gc) == 1)
		return ;
	if (ft_strchr(key, '+'))
		handle_plus(env, &key, &value, gc);
	new_node = create_env_node(key, value);
	add_node(env, new_node);
}
void var_set(char *str, t_env **env, t_gc **gc)
{
    char *key;
    char *value;
    size_t size;

    key = NULL;
    value= NULL;
    if(ft_strchr(str, '='))
    {
        size = ft_strchr(str, '=') - str + 1;
		key = ft_malloc(ft_substr(str, 0, size), gc, 0);
		value = ft_malloc(ft_substr(str, size, ft_strlen(str) - size), gc, 0);
        if(!key || !value)
            return ;
    }
    else
    {
		key = ft_malloc(ft_strdup(str), gc, 0);
        if(!key)
            return ;
    }
    add_var(env, key, value, gc);
}
