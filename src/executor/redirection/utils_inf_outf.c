#include "../../../includes/minishell.h"

int is_directory(char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) != 0)
        return (0);

    return (S_ISDIR(path_stat.st_mode));
}

int	ou_cas_p(t_redirection *ou, t_elem **elem, t_gc **gc)
{
	int	ou_fd;

	if(ou->is_ambigous)
	{
		printf("minishell :filename: ambiguous redirect\n");
		return 0;
	}
	if(is_directory(ou->file_or_delimiter))
	{
		printf("minishell: %s: Is a directory\n", ou->file_or_delimiter);
		return 0;
	}
	if (ou->type == REDIR_OUT)
		ou_fd = open(ou->file_or_delimiter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (ou->type == REDIR_APPEND)
		ou_fd = open(ou->file_or_delimiter, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (ou_fd == -1)
	{
		write(2, "minihell: ", 11);
		perror(ou->file_or_delimiter);
		return (0);
	}
	close(ou_fd);
	return (1);
}


int in_cas_p(t_redirection *in,  t_elem **elem, t_gc **gc)
{
    int	in_fd;
	
		if(in->is_ambigous)
	{
		printf("minishell :filename: ambiguous redirect\n");
		return 0;
	}
	if (in->type == REDIR_IN)
		in_fd = open(in->file_or_delimiter, O_RDONLY, 0644);
	else
		in_fd = in->herdoc_fd;
	if (in_fd == -1)
	{
		write(2, "minihell: ", 11);
		perror(in->file_or_delimiter);
		return 0;
	}
	ft_close(in_fd);
    return 1;
}
void ou_cas(t_redirection *ou, t_elem **elem, t_gc **gc)
{
	int ou_fd;
   
	if(ou->is_ambigous)
	{
		printf("minishell :filename: ambiguous redirect\n");
		exit(exit_stat(1, 1, gc, (*elem)->env));
	}
	if(is_directory(ou->file_or_delimiter))
	{
		printf("minishell: %s: Is a directory\n", ou->file_or_delimiter);
		exit(exit_stat(1, 1, gc, (*elem)->env));
	}
	if (ou->type == REDIR_OUT)
		ou_fd = open(ou->file_or_delimiter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (ou->type == REDIR_APPEND)
		ou_fd = open(ou->file_or_delimiter, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (ou_fd == -1)
	{
		perror(ou->file_or_delimiter);
		exit(exit_stat(1, 1, gc, (*elem)->env));
	}
	ft_dup2(ou_fd, 1, -1);
	ft_close(ou_fd);
}

void  in_cas(t_redirection *in,t_elem **elem, t_gc **gc)
{
	int	in_fd;

	if (in->type == REDIR_IN)
		in_fd = open(in->file_or_delimiter, O_RDONLY, 0644);
	else
		in_fd = in->herdoc_fd;
	if (in_fd == -1)
	{
		write(2, "minihell: ", 11);
		perror(in->file_or_delimiter);
		exit(exit_stat(1, 1, gc, (*elem)->env));
	}
	ft_dup2(in_fd, 0, -1);
	ft_close(in_fd);
}
