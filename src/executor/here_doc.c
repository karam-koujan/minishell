/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:30:39 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/27 00:11:25 by kkoujan          ###   ########.fr       */
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

char	*expand_herdoc(char *line, t_env *env)
{
	int		i;
	int		start;
	char	*var_name;
	char	*var_value;
	char	*result;
	char	*temp;
	int		var_start;
	char	*line_tmp;
	if (!line)
		return (NULL);
	
	result = ft_strdup("");  // Start with empty string
	i = 0;
	
	while (line[i])
	{
		if (line[i] == '$')
		{
			var_start = i;
			i++;
			start = i;
			// Extract variable name (alphanumeric + underscore)
			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
				i++;
			
			if (i > start)  // Found a valid variable name
			{
				var_name = ft_substr(line, start, i - start);
				var_value = ft_getenv_val(env, var_name);  // Get expanded value
				
				if (var_value)
				{
					// Append the expanded variable value to result
					temp = ft_strjoin(result, var_value);
					free(result);
					result = temp;
				}
				// If var_value is NULL, we simply don't add anything (variable expands to empty)
				free(var_name);
			}
			else
			{
				// Just a '$' without valid variable name, keep it as is
				temp = ft_strjoin(result, "$");
				free(result);
				result = temp;
			}
		}
		else
		{
			// Regular character, append to result
			line_tmp = ft_strdup(&line[i]);
			temp = ft_strjoin(result, line_tmp);
			free(line_tmp);
			free(result);
			result = temp;
			i++;
		}
	}
	
	free(line);  // Free original line
	return (result);
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
