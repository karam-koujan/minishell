/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:15:45 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/01 10:29:36 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_cmd_to_gc(t_gc **head, t_simple_cmd *cmd)
{
	int				i;
	t_redirection	*redir;

	i = -1;
	redir = cmd->redirs;
	while (++i < cmd->argc)
		gc_add_back(head, init_addr(cmd->args[i]));
	if (cmd->argc > 0)
		gc_add_back(head, init_addr(cmd->args));
	while (redir != NULL)
	{
		gc_add_back(head, init_addr(redir->file_or_delimiter));
		gc_add_back(head, init_addr(redir));
		redir = redir->next;
	}
	gc_add_back(head, init_addr(cmd));
}

void	add_parser_to_gc(t_gc **head, t_cmd_table **st)
{
	t_cmd_table	*tl;
	int			i;

	tl = *st;
	i = -1;
	while (++i < tl->cmd_count)
		add_cmd_to_gc(head, tl->cmds[i]);
	gc_add_back(head, init_addr(tl->cmds));
	gc_add_back(head, init_addr(tl));
}

void	add_token_to_gc(t_gc **head, t_token **st)
{
	t_token	*tl;

	tl = *st;
	while (tl != NULL)
	{
		gc_add_back(head, init_addr(tl->val));
		gc_add_back(head, init_addr(tl));
		tl = tl->next;
	}
}

void	add_env_to_gc(t_gc **head, t_env **st)
{
	t_env	*tl;

	tl = *st;
	while (tl != NULL)
	{
		gc_add_back(head, init_addr(tl->key));
		gc_add_back(head, init_addr(tl->value));
		gc_add_back(head, init_addr(tl));
		tl = tl->next;
	}
}

void	add_to_gc(t_gc **head, void	**st, int type)
{
	if (!head || !st)
		return ;
	if (!*st)
		return ;
	if (type == 0)
		add_token_to_gc(head, (t_token **)st);
	else if (type == 1)
		add_parser_to_gc(head, (t_cmd_table **)st);
	else if (type == 2)
		add_env_to_gc(head, (t_env **)st);
}
