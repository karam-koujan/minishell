/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:17:43 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/20 16:29:39 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

void update_pwd(t_env **env, char *key, char *value)
{
    t_env *current = *env;
    t_env *node;

    if (!key || !value)
        return ;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            free(key);
            current->value = value;
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
void builtin_cd_child(t_simple_cmd **data, t_env **env, t_gc **gc,t_elem **elem)
{
    char *target;
    char *oldpwd;
    char *newpwd;

    target = NULL;
   // write(2, "aaaaaaaaaaaa\n", 14);
    if((*data)->argc > 2)
        return(write(2, "minishell: cd: too many arguments\n", 35),
            (void)exit(exit_stat(1, 1, NULL, NULL)));
    oldpwd = getcwd(NULL, 0);
    if(!oldpwd)
        return (write(2, "minishell: cd: getcwd failed", 29),
            (void)exit(exit_stat(1, 1, gc, (*elem)->env)));
    target = cd_get_target(*data, (*env), gc);
    if (!target)
        return (free(oldpwd), (void)exit(exit_stat(1, 1,gc, (*elem)->env)));//add
    if (chdir(target) != 0)
    {
        write(2, "minishell: cd: ",16);
        write(2, target, ft_strlen(target));
        write(2, ": No such file or directory\n", 29);
        return (free(oldpwd), (void)exit(exit_stat(1, 1, NULL, NULL))); //add
    }
}
void builtin_cd(t_simple_cmd **data, t_env **env, t_gc **gc,t_elem **elem)
{
    char *target;
    char *oldpwd;
    char *newpwd;

    target = NULL;
    if((*data)->args[2])
        return(printf("minishell: cd: too many arguments\n"), (void)exit_stat(1, 1, NULL, NULL));
    oldpwd = getcwd(NULL, 0);
    if(!oldpwd)
        return (printf("minishell: cd: getcwd failed"), (void)exit(exit_stat(1, 1, gc, (*elem)->env)));
    target = cd_get_target(*data, (*env), gc);
    if (!target)
        return (free(oldpwd), (void)exit(exit_stat(1, 1,gc, (*elem)->env)));//add
    if (chdir(target) != 0)
        return (perror((*data)->args[1]), free(oldpwd), (void)exit_stat(1, 1, NULL, NULL)); //add
    update_pwd(env, ft_strdup("OLDPWD="), oldpwd);
    newpwd = getcwd(NULL, 0);
    if (newpwd)
        update_pwd(env, ft_strdup("PWD="), ft_strdup(newpwd));
}
