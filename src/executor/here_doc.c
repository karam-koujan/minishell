#include "../../includes/minishell.h"

static int read_in_stdin(int fd, char *delimiter)
{
	char	*line;

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
	close(fd);
	return (1);
}

int	here_doc(char *delimiter,  t_elem **elem, t_gc **gc)
{
	int		fd;
	pid_t	pid;
	int		status;

	fd = open("/tmp/.here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		if (g_gl == 3)
			return (exit(exit_stat(130, 1, gc, (*elem)->env)), -1);
			signal(SIGINT, SIG_DFL);
		if (!read_in_stdin(fd, delimiter))
			exit(exit_stat(1, 1, gc, (*elem)->env));
		exit(exit_stat(0, 1, gc, (*elem)->env));
	}
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			return (printf("\n"), g_gl = 3, exit_status(status), -1);
		exit_status(status);
		fd = open("/tmp/.here_doc", O_RDONLY);
		unlink("/tmp/.here_doc");
		return (fd);
	}
}
