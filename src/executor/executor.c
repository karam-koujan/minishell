/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:16:34 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 15:23:40 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include "../../includes/minishell.h"

void exec_builtin(t_simple_cmd **data, t_env **env, char **env_arr, t_gc **gc)
{
    if (!ft_strcmp((*data)->args[0], "cd"))
        builtin_cd(data, env, gc);
    else if (!ft_strcmp((*data)->args[0], "export"))
        builtin_export(data, env, gc);
    else if (!ft_strcmp((*data)->args[0], "exit"))
        builtin_exit(data);
    else if (!ft_strcmp((*data)->args[0], "unset"))
        builtin_unset(data, env);
}

int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
            || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "unset"));
}

void single_cmd(t_simple_cmd **data, t_env **env, char **env_arr, t_gc **gc)
{
    exit_stat(0, 1);
    if ((*data)->argc == 0)
        inf_outf_cmd(data, 0);
    else if (is_builtin((*data)->args[0]))
    {
        inf_outf_cmd(data, 0);
        exec_builtin(data, env, env_arr, gc);
    }
    else
        exec_cmd(data, *env, env_arr);
}

void exec(t_cmd_table *data, t_env **env, t_gc **gc)
{
    char **env_arr;

    if (!data || !env || !(*env))
        return; 

    env_arr = env_list_to_array(*env);  
    if (!env_arr)
        return; 

    if (data->cmd_count == 1)
        single_cmd(data->cmds, env, env_arr, gc);
    else if (data->cmd_count > 1)
        pipe_case(data, *env, env_arr);
    //free env_arr:
}
