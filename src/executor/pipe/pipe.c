/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:22:22 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/20 16:22:47 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../includes/minishell.h"

int first_proc(t_simple_cmd *cmd, t_env *env, char **env_arr)
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
		ft_close(fd[0]);
		ft_dup2(fd[1], STDOUT_FILENO, fd[1]);
		ft_close(fd[1]);
		exec_proc(&cmd, env, env_arr);
	}
	ft_close(fd[1]);
	return (fd[0]); 
}

int mid_proc(int fd_save, t_simple_cmd *cmd, t_env *env, char **env_arr)
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
		ft_close(fd[0]);
		ft_dup2(fd_save, STDIN_FILENO, fd[1]);
		ft_close(fd_save);
		ft_dup2(fd[1], STDOUT_FILENO, -1);
		ft_close(fd[1]);
		exec_proc(&cmd, env, env_arr);
	}
	ft_close(fd[1]);
	ft_close(fd_save);
	return (fd[0]); 
}

void last_proc(int fd_save, t_simple_cmd *cmd, t_env *env, char **env_arr)
{
	int child;

	child = fork();
	if (child == -1)
		return (perror("fork"), close(fd_save), (void)0);
	if (child == 0)
	{
		ft_dup2(fd_save, STDIN_FILENO, -1);
		ft_close(fd_save);
		exec_proc(&cmd, env, env_arr);
	}
	ft_close(fd_save);
	waitpid(child, NULL, 0);
}

void pipe_case(t_cmd_table *data, t_env *env, char ** env_arr)
{
	int fd_save;
	int i;

	i = 0;
	fd_save = first_proc(data->cmds[0], env, env_arr);
	if(fd_save == -1)
		return ;
	if(data->cmd_count >= 3)
	{
		while(i < data->cmd_count - 2)
		{
			fd_save =  mid_proc(fd_save, data->cmds[i + 1], env, env_arr);
			if(fd_save == -1)
			{
				while ((wait(NULL) != -1))
					;
				return ;
			}
			i++;
		}
	}
	last_proc(fd_save, data->cmds[data->cmd_count - 1], env, env_arr);
	while ((wait(NULL) != -1))
		;
}