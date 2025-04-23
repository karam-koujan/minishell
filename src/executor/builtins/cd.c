/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:17:43 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/22 15:46:17 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

char *ft_getenv(t_env *env, char *key)
{
    if(!env || !key)
        return NULL;
    while(env)
    {
        if(ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return NULL;// exit status is 1;
}
void update_pwd(t_env **env, char *key, char *value)
{
    t_env *current = *env;
    t_env *node;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            // you should free the previous current->value;
            free(current->value);
            current->value = ft_strdup(value);
            if(!current->value)
                    return ;// exit status is 0;
            return ;// exit status is 1;
        }
        current = current->next;
    }
    node = create_env_node(key, value);
    add_node(env, node);
}
char *cd_get_target(t_simple_cmd *cmd, t_env *env)
{
    char *target;

    if(!cmd->args[1])
    {
        target = ft_getenv(env, "HOME=");
        if(!target)
        {
            write(1, "minishell: cd: HOME not set\n", 29);
            return (NULL);// exit status is 1;
        }
    }
    else if (ft_strcmp(cmd->args[1], "-") == 0)  
    {
        target = ft_getenv(env, "OLDPWD=");
        if (!target)
            return (write(1, "minishell: cd: OLDPWD not set\n", 31), NULL);// exit status is 1;
    }  
    else 
        target = cmd->args[1];
    return (target);
}
void builtin_cd(t_simple_cmd **data, t_env *env)
{
    char *target;
    char *oldpwd;
    char *newpwd;
    
    if((*data)->args[2])
    {
        write(1, "minishell: cd: too many arguments\n", 35);
        return ;
    }
    target = NULL;
    oldpwd = getcwd(NULL, 0);
    if(!oldpwd)
        return ;// exit status is 1;
    target = cd_get_target(*data, env);
    if (!target)
        return ;// exit status is 1;
    if (chdir(target) != 0)
        return ( write(1, "minishell: ", 12),  perror((*data)->args[1])); 
    update_pwd(&env, "OLDPWD=", oldpwd);
    newpwd = getcwd(NULL, 0);
    if (newpwd)
        update_pwd(&env, "PWD=", newpwd);
}



