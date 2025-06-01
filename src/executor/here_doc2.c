/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:30:17 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/31 15:24:42 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_single_fd(t_simple_cmd	*cmd)
{
	t_redirection	*redir;

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
	if (!data || !*data)
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

static char	*append_suffix(char *name, size_t len)
{
	char	*tmp;

	while (access(name, F_OK | X_OK) == 0)
	{
		tmp = malloc(ft_strlen(name) + 2);
		if (!tmp)
		{
			free(name);
			return (NULL);
		}
		ft_strlcpy(tmp, name, len);
		tmp[len++] = 't';
		tmp[len] = '\0';
		free(name);
		name = tmp;
	}
	return (name);
}

static char	*resolve_name_conflict(char *base)
{
	char	*name;
	size_t	len;

	len = ft_strlen(base);
	name = ft_strdup(base);
	free(base);
	if (!name)
		return (NULL);
	return (append_suffix(name, len));
}

char	*gene_name_here_doc(void)
{
	static int	id = 0;
	char		*nbr;
	char		*base;
	char		*name;

	id = (id + 8754) * (8756 / 788) % 7851498555;
	nbr = ft_itoa(id);
	if (!nbr)
		return (NULL);
	base = ft_strjoin("/tmp/.here_doc_", nbr);
	free(nbr);
	if (!base)
		return (NULL);
	if (access(base, F_OK | X_OK) == 0)
		name = resolve_name_conflict(base);
	else
		name = base;
	id++;
	return (name);
}
