/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inf_outf_single_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:37:46 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/20 09:42:13 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../../includes/minishell.h"

int is_directory(char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) != 0)
        return (0);

    return (S_ISDIR(path_stat.st_mode));
}

int	ou_cas_p(t_redirection *ou)
{
	int	ou_fd;

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


int in_cas_p(t_redirection *in)
{
    int	in_fd;

	if (in->type == REDIR_IN)
		in_fd = open(in->file_or_delimiter, O_RDONLY, 0644);

	if (in_fd == -1)
	{
		write(2, "minihell: ", 11);
		perror(in->file_or_delimiter);
		return 0;
	}
	close(in_fd);
    return 1;
}
void ou_cas(t_redirection *ou)
{
    int ou_fd;

	if (is_directory(ou->file_or_delimiter))
	{
    	printf("minishell: %s: Is a directory\n", ou->file_or_delimiter);
		exit(126);
    	//exit_status = 126; // Command is not executable
   		return;
	}
    if (ou->type == REDIR_OUT)
		ou_fd = open(ou->file_or_delimiter, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (ou->type == REDIR_APPEND)
		ou_fd = open(ou->file_or_delimiter, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (ou_fd == -1)
	{
		
		perror(ou->file_or_delimiter);
		exit(1);
	}
    if (dup2(ou_fd, 1) == -1)
    {
        perror("minihell: ");
		close(ou_fd);
        exit(1);
    }
	close(ou_fd);
}

void  in_cas(t_simple_cmd **data,  t_redirection *in)
{
    int in_fd;

	if (is_directory(in->file_or_delimiter))
	{
    	printf("%s: -: Is a directory\n", (*data)->args[0]);
		exit(1);
    	//exit_status = 126; // Command is not executable
   		return;
	}
    if(in->type == REDIR_IN)
    in_fd = open(in->file_or_delimiter, O_RDONLY, 0644);
    if(in_fd == -1)
    {
        write(2, "minihell: ", 11);
		perror(in->file_or_delimiter);
        exit(1);
    }
    close(0);
	if (dup(in_fd) == -1)
	{
		perror("minihell: ");
		close(in_fd);
        exit(1);
    }
    close(in_fd);
}

int inf_outf_cmd(t_simple_cmd **data, int flag)
{
    t_redirection *files;

    files =  (*data)->redirs;
    if(flag == 1)
    {
        while(files)
        {
            if(files->type == REDIR_IN || files->type == REDIR_HEREDOC)
                in_cas(data, files);
            else if(files->type == REDIR_OUT || files->type == REDIR_APPEND)
                ou_cas(files);
            files = files->next;
        }
    }
    else if (flag == 0)
    {
        while (files)
		{
			if (files->type == REDIR_IN || files->type == REDIR_HEREDOC)
			{
				if (in_cas_p(files) == 0)
				{
					exit(1);
					return (1);
				}
			}
			else if (files->type == REDIR_OUT || files->type == REDIR_APPEND)
			{
				if (ou_cas_p(files) == 0)
				{
					exit(1);
					return (1);
				}
			}
			files = files->next;
		}
    }
	return 0;
}