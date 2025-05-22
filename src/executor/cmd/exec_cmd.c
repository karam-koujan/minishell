/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:45 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/22 14:40:38 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void check_is_building(t_simple_cmd **data, t_env *env, t_elem **elem, t_gc **gc)
{
	if((*data)->argc == 0)
		exit(exit_stat(1, 1, gc, (*elem)->env));
	if(ft_strcmp((*data)->args[0], "echo") == 0)
	{
		builtin_echo(data);
		exit(exit_stat(0, 1, gc, (*elem)->env));
	}
	if(ft_strcmp((*data)->args[0], "pwd") == 0)
	{
		builtin_pwd(data);
		exit(exit_stat(0, 1, gc, (*elem)->env));
	}
	if(ft_strcmp((*data)->args[0], "env") == 0)
	{
		builtin_env(data, env);
		exit(exit_stat(0, 1, gc,(*elem)->env));
	}
	if(ft_strcmp((*data)->args[0], "export") == 0)
	{
		builtin_export(data, &env, gc);
		exit(exit_stat(0, 1, gc, (*elem)->env));
	}
	if(ft_strcmp((*data)->args[0], "unset") == 0)
		exit(exit_stat(0, 1, gc, (*elem)->env));
	if(ft_strcmp((*data)->args[0], "exit") == 0)
		exit(exit_stat(0, 1, gc, (*elem)->env));
	if(ft_strcmp((*data)->args[0], "cd") == 0)
		exit(exit_stat(0, 1, gc, (*elem)->env));
}
void exec_proc(t_simple_cmd **data, t_env *env, t_elem **elem, t_gc **gc)
{
	inf_outf_cmd(data, 1, elem, gc);
		//printf("here %s\n", (*data)->args[0]);
		check_is_building(data, env,  elem, gc);
		check_exec_cmd((*data)->args, elem, gc);

}

void exec_cmd(t_simple_cmd **data,t_env *env, t_elem **elem, t_gc **gc)
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
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		exec_proc(data, env, elem, gc);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (!WIFSIGNALED(status))
			exit_status(status);
		else if (exit_stat(0, 0, NULL, NULL) == 130)
			printf("\n");
		else if (status == 131 && printf("\n"))
			exit_stat(status, 1, NULL, NULL);
	}
}
