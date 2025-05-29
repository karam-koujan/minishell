/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:10:55 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/29 21:12:27 by kkoujan          ###   ########.fr       */
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
			herdoc_data.temp = ft_strjoin(herdoc_data.result, \
				herdoc_data.line_tmp);
			free(herdoc_data.line_tmp);
			free(herdoc_data.result);
			herdoc_data.result = herdoc_data.temp;
			(herdoc_data.i)++;
		}
	}
	free(line);
	return (herdoc_data.result);
}
