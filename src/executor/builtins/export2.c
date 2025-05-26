/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:20:12 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/26 17:18:39 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static t_env	*env_node(t_env *env)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(env->key);
	if (env->value)
		new_node->value = ft_strdup(env->value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

static t_env	*copy_env(t_env *env)
{
	t_env	*new_env;
	t_env	*last;
	t_env	*new_node;

	new_env = NULL;
	last = NULL;
	new_node = NULL;
	while (env)
	{
		new_node = env_node(env);
		if (!new_node)
			return (NULL);
		if (!new_env)
			new_env = new_node;
		else
			last->next = new_node;
		last = new_node;
		env = env->next;
	}
	return (new_env);
}

static t_env	*sort_var(t_env *head)
{
	t_env	*curr;
	char	*swap_key;
	char	*swap_value;

	if (!head)
		return (NULL);
	curr = head;
	while (curr && curr->next)
	{	
		if (ft_strcmp(curr->key, curr->next->key) > 0)
		{
			swap_key = curr->key;
			curr->key = curr->next->key;
			curr->next->key = swap_key;
			swap_value = curr->value;
			curr->value = curr->next->value;
			curr->next->value = swap_value;
			curr = head;
		}
		else
			curr = curr->next;
	}
	return (head);
}

void	print_export(t_env *env)
{
	t_env	*sorted;
	t_env	*copy;
	int		i;

	i = 0;
	copy = copy_env(env);
	sorted = sort_var(copy);
	if (!sorted)
		return (free_env(&copy, free));
	while (sorted)
	{
		if (i == 1 && sorted->key && ft_strcmp(sorted->key, "PATH=") == 0)
		{
			sorted = sorted->next;
			i++;
			continue ;
		}
		else if (!sorted->value)
			printf("declare -x %s\n", sorted->key);
		else
			printf("declare -x %s\"%s\"\n", sorted->key, sorted->value);
		sorted = sorted->next;
		i++;
	}
	free_env(&copy, free);
}
