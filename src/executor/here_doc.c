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
		// Optional: expand variables if delimiter is unquoted
		// if (!replace_dollar(&line))
		//     return false;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (1);
}

int	here_doc(char *delimiter)
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
		signal(SIGINT, SIG_DFL);
		if (!read_in_stdin(fd, delimiter))
			exit(exit_stat(1, 1));
		exit(exit_stat(0, 1));
	}
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			return (-1);
		fd = open("/tmp/.here_doc", O_RDONLY);
		unlink("/tmp/.here_doc");
		exit_stat(status, 1);
		return (fd);
	}
}
