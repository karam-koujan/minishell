/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:32 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/19 18:36:37 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

void unset_env(char *key, t_env **env, t_gc **gc)
{
    t_env *prev;
    t_env *curr;
    char *key_env;

    prev = NULL;
    curr = *env;
    if(ft_strchr(key, '='))
        return ;
    while (curr) 
    {
        key_env = ft_malloc(ft_strtrim(curr->key, "="), gc, 0);
        if(!key_env)
            return ;
        if (ft_strcmp(key_env, key) == 0) 
        {
            if (prev)
                prev->next = curr->next;
            else
                *env = curr->next;
            return(free(curr->key), free(curr->value),
                       free(curr));
        }
        prev = curr;
        curr = curr->next;
    }
}

void  builtin_unset(t_simple_cmd **data, t_env **env, t_gc **gc)
{
    int i;
 
    i = 1;
    while((*data)->args[i])
    {
       unset_env((*data)->args[i], env, gc);
        i++;
    }
}
