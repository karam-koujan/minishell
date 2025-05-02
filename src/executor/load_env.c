/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:23:12 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 11:57:59 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

int *init_empty_env(t_env **env)
{
    char *path;
    char *value;
    t_env *new_node;

    path = ft_strdup("PATH=");
    value = ft_strdup(PATH);
    new_node = create_env_node(path, value);
    add_node(env, new_node);
}
static void get_key_value(char *envp, char **key, char **value)
{
    size_t equal_signe;
    
    equal_signe = ft_strchr(envp, '=') - envp + 1;
    (*key) = ft_substr(envp, 0, equal_signe);
    (*value) = ft_substr(envp, equal_signe, ft_strlen(envp) - 1);
}
t_env *init_env_list(char **envp)
{
    t_env *head;
    t_env *new_node;
    t_env *current;
    char *key;
    char *value;
    int i;

    head = NULL;
    current = NULL;
    new_node = NULL;
    i = 0;
    while(envp[i])
    {
        get_key_value(envp[i], &key, &value);
        if(!key || !value)
            return(free(key), free(value), NULL);
        new_node = create_env_node(key , value);
        if(!new_node)
            return (free(key), free(value), NULL);
        add_node(&head, new_node);
        i++;
    }
    return(head);
}
