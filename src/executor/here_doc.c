/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:30:39 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/29 21:12:10 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	read_in_stdin(int fd, t_redirection *redir, t_env *env)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("warning: here-document delimited by end-of-file ");
			printf("(wanted '");
			printf("%s", redir->file_or_delimiter);
			printf("')\n");
			break ;
		}
		if (ft_strcmp(line, redir->file_or_delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!redir->in_qt)
			line = expand_herdoc(line, env);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (1);
}

static int	child_here_doc(t_redirection *redir, int fd, t_gc **gc, char *name)
{
	free(name);
	signal(SIGINT, SIG_DFL);
	if (!read_in_stdin(fd, redir, (*gc)->env))
		exit(exit_stat(1, 1, gc, NULL));
	exit(exit_stat(0, 1, gc, NULL));
}

static int	parent_here_doc(pid_t pid, char *name)
{
	int	status;
	int	fd;

	waitpid(pid, &status, 0);
	if (g_gl == 3)
	{
		printf("\n");
		exit_stat(0, 0, NULL, NULL);
		g_gl = 3;
		return (free(name), -1);
	}
	exit_status(status);
	fd = open(name, O_RDONLY);
	unlink(name);
	return (free(name), fd);
}

int	here_doc(t_redirection *redir, t_elem **elem, t_gc **gc)
{
	int		fd;
	pid_t	pid;
	char	*name;

	if (g_gl != 3)
		g_gl = 4;
	else if (g_gl == 3)
		return (-1);
	name = gene_name_here_doc();
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(name), -1);
	pid = fork();
	if (pid == 0)
		child_here_doc(redir, fd, gc, name);
	close(fd);
	return (parent_here_doc(pid, name));
}
