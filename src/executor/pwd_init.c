/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:23:22 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/18 17:30:28 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"


void init_pwd(t_env **env, t_gc **gc)
{
    t_env *curr;
    t_env *new_node;
    char *pwd;
    int flag;

    curr = *env;
    flag = 0;
    while(curr)
    {
        if(ft_strcmp(curr->key, "PWD=") == 0)
            return ;
        if(ft_strcmp(curr->key, "OLDPWD") == 0)
            flag = 1;
        curr = curr->next;
    }
    pwd = getcwd(NULL, 0);
    if(!pwd)
        return ;//add
    new_node = create_env_node(ft_strdup("PWD="), pwd);
    if(!new_node)
        return (free(pwd));//add
    add_node(env, new_node);
    if(flag == 0)
    {
        new_node = create_env_node(ft_strdup("OLDPWD"), NULL);
        if(new_node)
            return ;//add
        add_node(env, new_node);
    }
}
