#include "../../includes/minishell.h"



void	handle_herdoc(t_cmd_table **data, t_elem *elem, t_gc **gc)
{
	int				i;
	t_redirection	*redir;

	i = -1;
	if (!data && !*data)
		return ;
	while (++i < (*data)->cmd_count)
	{
		redir = (*data)->cmds[i]->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				redir->herdoc_fd = here_doc(redir->file_or_delimiter, &elem, gc);					
			}
			redir = redir->next;
		}
	}
}

void	close_single_fd(t_simple_cmd	*cmd)
{
	int				i;
	t_redirection	*redir;

	i = -1;
	if (!cmd)
		return ;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->herdoc_fd != -1)
		{
			ft_close(redir->herdoc_fd);
			redir->herdoc_fd = -1;
		}
		redir = redir->next;
	}
}
void	close_pipe_fd(t_cmd_table **data)
{
	int				i;
	t_redirection	*redir;

	i = -1;
	if (!data && !*data)
		return ;
	while (++i < (*data)->cmd_count)
	{
		redir = (*data)->cmds[i]->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && redir->herdoc_fd != -1)
			{
				ft_close(redir->herdoc_fd);
				redir->herdoc_fd = -1;
			}
			redir = redir->next;
		}
	}
}


static int read_in_stdin(int fd, char *delimiter)
{
	char	*line;
	int		dev_in_fd;

	// close(0);
	// dev_in_fd = open("/dev/tty", O_RDONLY);
	// if (!dev_in_fd)
	// 	return (-1);
	// close(1);
	// dev_in_fd = open("/dev/tty", O_RDONLY);
	// if (!dev_in_fd)
	// 		return (-1);
	// write(2, "Adad", 4);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("warning: here-document delimited by end-of-file ");
			printf("(wanted '");
			printf("%s",delimiter);
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

int	here_doc(char *delimiter,  t_elem **elem, t_gc **gc)
{
	int		fd;
	pid_t	pid;
	int		status;

	if (g_gl != 3)
		g_gl = 4;
	fd = open("/tmp/.here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		if (g_gl == 3)
			return (exit(exit_stat(0, 0, gc, NULL)), -1);
		signal(SIGINT, SIG_DFL);
		if (!read_in_stdin(fd, delimiter))
			exit(exit_stat(1, 1, gc, NULL));
		exit(exit_stat(0, 1, gc, NULL));
	}
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (g_gl == 3) 
			return (printf("%i\n", exit_stat(0, 0, NULL, NULL)), g_gl = 3 , -1);
		printf("status %i\n", status);
		exit_status(status);
		fd = open("/tmp/.here_doc", O_RDONLY);
		unlink("/tmp/.here_doc");
		return (fd);
	}
}
char *gene_name_here_doc()
{
	static int id = 0;
	char *nbr;
	char *base_name;
	char *name;
	char *tmp_name;
	size_t len ;

	  id = (id + 8754) * (8756 / 788) % 7851498555;
	  nbr = ft_itoa(id);
	  if(!nbr)
	  	return NULL;
	base_name = ft_strjoin("/tmp/.here_doc_", nbr);
	free(nbr);
	if(!base_name)
		return NULL;
	if(access(base_name, F_OK | X_OK) == 0)
	{
		len = ft_strlen(base_name);
		name = ft_strdup(base_name);
		if(!name)
			return (free(base_name), NULL);
		free(base_name);
		while(access(name, F_OK | X_OK) == 0)
		{
			tmp_name = malloc(ft_strlen(name) + 2);
			if(!tmp_name)
				return (free(name), NULL);
			ft_strlcpy(tmp_name, name, len);
			tmp_name[len++] = 't';
			tmp_name[len] = '\0';
			free(name);
			name = tmp_name;
		}
	}
	else
		name = base_name;
	id++;
	return name;
	

}
// int here_doc(char *delimiter, t_elem **elem, t_gc **gc)
// {
// 	int fd; 
// 	char *name;

// 	name = gene_name_here_doc();
// 	if(!name)
// 		return -1;
// 	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (free(name), -1);
// 	if(!read_in_stdin(fd, delimiter))
// 		return (exit_stat(1, 1, NULL, NULL), ft_close(fd), free(name), -1);//free data
// 	ft_close(fd);
// 	fd = open(name, O_RDONLY);
// 	if(fd < 0)
// 		return (exit_stat(1, 1, NULL, NULL), free(name), -1);//free data
// 	unlink(name);
// 	free(name);
// 	return (fd);
// }