/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:23:12 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 14:46:03 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

void init_empty_env(t_env **env, t_gc **gc)
{
    char *path;
    char *value;
    t_env *new_node;

    path = ft_malloc(ft_strdup("PATH="), gc, 0);
	if (!path)
		return (free_all(gc));
	value = ft_malloc(ft_strdup(PATH), gc, 0);
	if (!value)
		return ;
	new_node = create_env_node(path, value);
	if (!new_node)
		return free_all(gc);
	add_node(env, new_node);
}
static int	get_key_value(char *envp, char **key, char **value, t_gc **gc)
{
	size_t	equal_sign;

	if (!envp  || !key || !value)
		return (0);
	equal_sign = ft_strchr(envp, '=') - envp + 1;
	*key = ft_malloc(ft_substr(envp, 0, equal_sign), gc, 0);
	*value = ft_malloc(ft_substr(envp, equal_sign,
				ft_strlen(envp) - equal_sign), gc, 0);
	if (!*key || !*value)
		return (0);
	return (1);
}

t_env	*init_env_list(char **envp, t_gc **gc)
{
	t_env	*head;
	t_env	*new_node;
	char	*key;
	char	*value;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		if (!get_key_value(envp[i], &key, &value, gc))
			return (NULL);
		new_node = create_env_node(key, value);
		if (!new_node)
			return (NULL);
		add_node(&head, new_node);
		i++;
	}
	return (head);
}
