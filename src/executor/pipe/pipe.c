/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:22:22 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/19 15:28:09 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"

int first_proc(t_simple_cmd *cmd, t_env *env, t_elem **elem, t_gc **gc)
{
	int fd[2];
	int child;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	child = fork();
	if (child == -1)
		return (perror("fork"), close(fd[0]), close(fd[1]), -1);
	if (child == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		ft_close(fd[0]);
		ft_dup2(fd[1], STDOUT_FILENO, fd[1]);
		ft_close(fd[1]);
		exec_proc(&cmd, env, elem, gc);
		free_arr((*elem)->env);
		clear_parsing(gc);
	}
	ft_close(fd[1]);
	return (fd[0]);
}

int mid_proc(t_simple_cmd *cmd, t_env *env, t_elem **elem, t_gc **gc)
{
	int fd[2];
	int child;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	child = fork();
	if (child == -1)
		return (perror("fork"), close(fd[0]), close(fd[1]), -1);
	if (child == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		ft_close(fd[0]);
		ft_dup2((*elem)->fd_save, STDIN_FILENO, fd[1]);
		ft_close((*elem)->fd_save);
		ft_dup2(fd[1], STDOUT_FILENO, -1);
		ft_close(fd[1]);
		exec_proc(&cmd, env, elem, gc);
		free_arr((*elem)->env);
		clear_parsing(gc);
	}
	ft_close(fd[1]);
	ft_close((*elem)->fd_save);
	return (fd[0]); 
}

void last_proc(t_simple_cmd *cmd, t_env *env, t_elem **elem, t_gc **gc)
{
	int child;
	int status;

	child = fork();
	if (child == -1)
		return (perror("fork"), close((*elem)->fd_save), (void)0);
	if (child == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		ft_dup2((*elem)->fd_save, STDIN_FILENO, -1);
		ft_close((*elem)->fd_save);
		exec_proc(&cmd, env, elem, gc);
		free_arr((*elem)->env);
		clear_parsing(gc);
	}
	ft_close((*elem)->fd_save);
	waitpid(child, &status, 0);
	if (status == 0)
		g_gl = 2;
	if (exit_stat(0, 0, NULL, NULL) == 130 || status == 131)
		printf("\n");
	exit_status(status);
}

void pipe_case(t_cmd_table *data, t_env *env, t_elem *elem, t_gc **gc)
{
	int i;
	int status;

	i = 0;
	elem->fd_save = first_proc(data->cmds[0], env, &elem, gc);
	if(elem->fd_save == -1)
		return ;
	if(data->cmd_count >= 3)
	{
		while(i < data->cmd_count - 2)
		{
			elem->fd_save =  mid_proc(data->cmds[i + 1], env, &elem, gc);
			if(elem->fd_save == -1)
			{
				exit_stat(1, 1, NULL, NULL);   
				while ((wait(&status) != -1))
					;
				return ;
			}
			i++;
		}
	}
	last_proc(data->cmds[data->cmd_count - 1], env, &elem, gc);
	while ((wait(NULL) != -1))
		;
}
