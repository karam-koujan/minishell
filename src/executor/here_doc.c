/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:30:39 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/29 18:08:04 by kkoujan          ###   ########.fr       */
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
				if (g_gl == 3)
					return ;
				redir->herdoc_fd = here_doc(redir,
						&elem, gc);
			}
			redir = redir->next;
		}
	}
}
void	join_expnd_herdoc(t_exp_heredoc *d, char *line, t_env *env)
{
	d->var_start = d->i;
	(d->i)++;
	d->start = d->i;
	while (line[d->i] && (ft_isalnum(line[d->i]) || line[d->i] == '_'))
		(d->i)++;
	if (d->i > d->start)
	{
		d->var_name = ft_substr(line, d->start, d->i - d->start);
		d->var_value = ft_getenv_val(env, d->var_name);
		if (d->var_value)
		{
			d->temp = ft_strjoin(d->result, d->var_value);
			free(d->result);
			d->result = d->temp;
		}
		free(d->var_name);
		free(d->var_value);
	}
	else
	{
		d->temp = ft_strjoin(d->result, "$");
		free(d->result);
		d->result = d->temp;
	}
}

char	*expand_herdoc(char *line, t_env *env)
{
	t_exp_heredoc	herdoc_data;
	if (!line)
		return (NULL);
	herdoc_data.result = ft_strdup("");
	herdoc_data.i = 0;
	while (line[herdoc_data.i])
	{
		if (line[herdoc_data.i] == '$')
			join_expnd_herdoc(&herdoc_data, line, env);
		else
		{
			herdoc_data.line_tmp = ft_substr(line, herdoc_data.i, 1);
			herdoc_data.temp = ft_strjoin(herdoc_data.result, herdoc_data.line_tmp);
			free(herdoc_data.line_tmp);
			free(herdoc_data.result);
			herdoc_data.result = herdoc_data.temp;
			(herdoc_data.i)++;
		}
	}
	free(line); 
	return (herdoc_data.result);
}

static int	read_in_stdin(int fd, t_redirection *redir, t_env *env)
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
			printf("%s", redir->file_or_delimiter);
			printf("')\n");
			break ;
		}
		if (!redir->in_qt)
			line = expand_herdoc(line, env);
		if (ft_strcmp(line, redir->file_or_delimiter) == 0)
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
