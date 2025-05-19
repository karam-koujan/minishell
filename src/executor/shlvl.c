/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:23:30 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/18 17:34:36 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

void update_shlvl(t_env **env, char *key, char *value)
{
    t_env *curr;

    curr = *env;
    while(curr)
    {
        if(ft_strcmp(curr->key, key) == 0)
        {
			free(curr->value);
            curr->value = value;
            return ;
        }
        curr = curr->next;
    }
	free(value);
}

int is_valid_number(char *str)
{
    if (!str || !*str)
        return 0;

    int i = 0;

    if (str[0] == '-' || str[0] == '+')
        i++;

    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}
static char	*get_new_shlvl(char *shlvl_str, t_gc **gc)
{
	int		shlvl;

	if (!is_valid_number(shlvl_str))
		return (ft_strdup("1"));
	shlvl = ft_atoi(shlvl_str) + 1;
	if (shlvl < 0)
		shlvl = 0;
	if (shlvl > 999)
	{
		printf("warning: shell level (1000) too high, resetting to 1\n");
		shlvl = 1;
	}
	return (ft_itoa(shlvl));
}

void	handle_shlvl(t_env **env, t_gc **gc)
{
	t_env	*new_node;
	char	*shlvl_str;
	char	*new_shlvl;
	char	*key;

	shlvl_str = ft_getenv(*env, "SHLVL=");
	if (shlvl_str)
	{
		new_shlvl = get_new_shlvl(shlvl_str, gc);
		if (!new_shlvl)
			return ;
		update_shlvl(env, "SHLVL=", new_shlvl);
	}
	else
	{
		key = ft_strdup("SHLVL=");
		new_shlvl = ft_strdup("1");
		if(!new_node)
			return (free(key));//add
		new_node = create_env_node(key, new_shlvl);
		if(!new_node)
			return(free(key), free(new_shlvl));//add
		add_node(env, new_node);
	}
}

