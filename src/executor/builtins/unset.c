/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:32 by achemlal          #+#    #+#             */
/*   Updated: 2025/06/01 11:56:29 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static void	unset_env(char *key, t_env **env)
{
	t_env	*prev;
	t_env	*c;
	char	*key_e;

	prev = NULL;
	c = *env;
	if (ft_strchr(key, '='))
		return ;
	while (c)
	{
		key_e = ft_strtrim(c->key, "=");
		if (!key_e)
			return ;
		if (ft_strcmp(key_e, key) == 0)
		{
			if (prev)
				prev->next = c->next;
			else
				*env = c->next;
			return (free(c->key), free(key_e), free(c->value), free(c));
		}
		prev = c;
		c = c->next;
		free(key_e);
	}
}

void	builtin_unset(t_simple_cmd **data, t_env **env, t_gc **gc)
{
	int	i;

	i = 1;
	while ((*data)->args[i])
	{
		unset_env((*data)->args[i], env);
		i++;
	}
	(*gc)->env = *env;
}
