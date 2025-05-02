/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:45 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/02 18:24:33 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void check_is_building(t_simple_cmd **data, t_env *env, char **env_arr)
{
    if(!(*data)->args[0])
        exit(exit_stat(1, 1));
    if(ft_strcmp((*data)->args[0], "echo") == 0)
    {
        builtin_echo(data);
        exit(exit_stat(0, 1));
    }
    if(ft_strcmp((*data)->args[0], "pwd") == 0)
    {
        builtin_pwd(data);
        exit(exit_stat(0, 1));
    }
    if(ft_strcmp((*data)->args[0], "env") == 0)
    {
        builtin_env(data, env);
        exit(exit_stat(0, 1));
    }
    if(ft_strcmp((*data)->args[0], "export") == 0)
        exit(exit_stat(0, 1));
    if(ft_strcmp((*data)->args[0], "unset") == 0)
        exit(exit_stat(0, 1));
    if(ft_strcmp((*data)->args[0], "exit") == 0)
        exit(exit_stat(0, 1));
    if(ft_strcmp((*data)->args[0], "cd") == 0)
        exit(exit_stat(0, 1));
}
void exec_proc(t_simple_cmd **data, t_env *env, char **env_arr)
{
    exit_stat(0, 1);
    inf_outf_cmd(data, 1);
    check_is_building(data, env,  env_arr);
    check_exec_cmd((*data)->args, env_arr);
}

void exec_cmd(t_simple_cmd **data,t_env *env, char **env_arr)
{
    pid_t pid;
    int status;

    if (!(*data)->args || !(*data)->args[0])
        return ;

    pid = fork();
    if (pid == -1)
        return (perror("Error: fork"));

    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        exec_proc(data, env, env_arr);
    }
    else
    {
        waitpid(pid, &status, 0);
        exit_status(status);
    }
}
