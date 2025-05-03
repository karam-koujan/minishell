/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:23:22 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 14:35:59 by achemlal         ###   ########.fr       */
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
    pwd = ft_malloc(getcwd(NULL, 0), gc, 0);
    new_node = create_env_node(ft_malloc(ft_strdup("PWD="), gc, 0), pwd);
    add_node(env, new_node);
    if(flag == 0)
    {
        new_node = create_env_node(ft_malloc(ft_strdup("OLDPWD"), gc, 0), NULL);
        add_node(env, new_node);
    }
}
