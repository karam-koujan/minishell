/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:17:43 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/18 17:58:55 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

void update_pwd(t_env **env, char *key, char *value)
{
    t_env *current = *env;
    t_env *node;
    
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            if(!current->value)
                    return ;
            return ;
        }
        current = current->next;
    }
    node = create_env_node(key, value);
    add_node(env, node);
}
char *cd_get_target(t_simple_cmd *cmd, t_env *env, t_gc **gc)
{
    char *target;

    if(!cmd->args[1] || ft_strcmp(cmd->args[1], "--") == 0)
    {
        target = ft_getenv(env, "HOME=");
        if(!target)
            return (printf("minishell: cd: HOME not set\n"), NULL);
    }
    else if (ft_strcmp(cmd->args[1], "-") == 0)  
    {
        target = ft_getenv(env, "OLDPWD=");
        if (!target)
            return (printf("minishell: cd: OLDPWD not set\n"), NULL);
        else
            return(printf("%s\n", target), target);
    }  
    else 
        target = cmd->args[1];
    return (target);
}
void builtin_cd(t_simple_cmd **data, t_env **env, t_gc **gc,t_elem **elem)
{
    char *target;
    char *oldpwd;
    char *newpwd;

    target = NULL;
    if((*data)->args[2])
        return(printf("minishell: cd: too many arguments\n"), (void)exit_stat(1, 1, gc, (*elem)->env));
    oldpwd = getcwd(NULL, 0);
    if(!oldpwd)
        return (printf("minishell: cd: getcwd failed"), (void)exit(exit_stat(1, 1, gc, (*elem)->env)));
    target = cd_get_target(*data, (*env), gc);
    if (!target)
        return (free(oldpwd), (void)exit(exit_stat(1, 1,gc, (*elem)->env)));//add
    if (chdir(target) != 0)
        return (printf("minishell: "),  
            perror((*data)->args[1]), free(oldpwd), (void)exit_stat(1, 1, gc, (*elem)->env)); //add
    update_pwd(env, ft_strdup("OLDPWD="), oldpwd);
    free(oldpwd);//add
    newpwd = getcwd(NULL, 0);
    if (newpwd)
        update_pwd(env, ft_strdup("PWD="), newpwd);
    free(newpwd);
}
