/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:46:19 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 18:46:35 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include  "../../../includes/minishell.h"


static char	*prepare_key(char *key, t_gc **gc)
{
	char	*trimmed;
	char	*joined;

	trimmed = ft_strtrim(key, "+=");
	if (!trimmed)
		return (NULL);
	joined = ft_strjoin(trimmed, "=");
	free(trimmed);
	if (!joined)
		return (NULL);
	return (ft_malloc(joined, gc, 0));
}

static int	append_if_exists(t_env *env, char *key, char *value, t_gc **gc)
{
	char	*new_val;

	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			new_val = ft_strjoin(env->value, value);
			if (!new_val)
				return (1);
			env->value = ft_malloc(new_val, gc, 0);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	handle_plus(t_env **env, char **key, char **value, t_gc **gc)
{
	t_env	*new_node;
	char	*new_key;

	if (!*key)
		return ;
	new_key = prepare_key(*key, gc);
	if (!new_key)
		return ;
	*key = new_key;
	if (append_if_exists(*env, *key, *value, gc))
		return ;
	new_node = create_env_node(*key, *value);
	add_node(env, new_node);
}