/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:30:39 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 13:16:51 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_herdoc(t_cmd_table **data, t_elem *elem, t_gc **gc)
{
	int				i;
	t_redirection	*redir;

	i = -1;
	if (!data || !*data)
		return ;
	while (++i < (*data)->cmd_count)
	{
		redir = (*data)->cmds[i]->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				redir->herdoc_fd = here_doc(redir->file_or_delimiter,
						&elem, gc);
			}
			redir = redir->next;
		}
	}
}

static int	read_in_stdin(int fd, char *delimiter)
{
	char	*line;
	int		dev_in_fd;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("warning: here-document delimited by end-of-file ");
			printf("(wanted '");
			printf("%s", delimiter);
			printf("')\n");
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (1);
}

static int	child_here_doc(char *delimiter, int fd, t_gc **gc)
{
	if (g_gl == 3)
	{
		ft_close(fd);
		exit(exit_stat(0, 0, gc, NULL));
	}
	signal(SIGINT, SIG_DFL);
	if (!read_in_stdin(fd, delimiter))
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
		return (-1);
	}
	exit_status(status);
	fd = open(name, O_RDONLY);
	unlink(name);
	return (fd);
}

int	here_doc(char *delimiter, t_elem **elem, t_gc **gc)
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
		return (-1);
	pid = fork();
	if (pid == 0)
		child_here_doc(delimiter, fd, gc);
	close(fd);
	return (parent_here_doc(pid, name));
}
