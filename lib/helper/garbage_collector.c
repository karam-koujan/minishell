/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:02:08 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/30 12:09:21 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_gc	*init_addr(void	*addr)
{
	t_gc	*gc;

	gc = malloc(sizeof(gc));
	if (gc == NULL)
		return (NULL);
	gc->addr = addr;
	gc->next = NULL;
	return (gc);
}

void	gc_add_back(t_gc **head, t_gc *new)
{
	t_gc	*l;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
	}
	else
	{
		l = *head;
		while (l->next != NULL)
		{
			l = l->next;
		}
		l->next = new;
	}
}

void	free_all(t_gc **head)
{
	t_gc	*l;
	t_gc	*tmp;

	if (!head)
		return ;
	if (!(*head))
		return ;
	l = *head;
	while (l != NULL)
	{
		free(l->addr);
		tmp = l;
		l = l->next;
		free(tmp);
		tmp = NULL;
	}
}

void	add_token_to_gc(t_gc **head, t_token **st)
{
	t_token	*tl;
	t_gc	*gc;

	tl = *st;
	while (tl != NULL)
	{
		gc_add_back(head, init_addr(tl->val));
		gc_add_back(head, init_addr(tl));
		tl = tl->next;
	}
}

void	add_cmd_to_gc(t_gc **head, t_simple_cmd *cmd)
{
	int				i;
	t_redirection	*redir;

	i = -1;
	redir = cmd->redirs;
	while (++i < cmd->argc)
	gc_add_back(head, init_addr(cmd->args[i]));
	if (cmd->argc > 0)
		gc_add_back(head, init_addr(cmd));
	while (redir != NULL)
	{
		gc_add_back(head, init_addr(redir->file_or_delimiter));
		gc_add_back(head, init_addr(redir));
		redir = redir->next;
	}
}

void	add_parser_to_gc(t_gc **head, t_cmd_table **st)
{
	t_cmd_table	*tl;
	t_gc	*gc;
	int		i;

	tl = *st;
	i = -1;
	while (++i < tl->cmd_count)
		add_cmd_to_gc(head, tl->cmds[i]);
	gc_add_back(head, init_addr(tl->cmds));
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
}

void	*ft_malloc(size_t size, t_gc **head, int enable_free)
{
	void	*ptr;
	t_gc	*gc;

	if (enable_free)
		return (free_all(head), NULL);
	ptr = malloc(size);
	if (ptr == NULL)
		return (NULL);
	gc = init_addr(ptr);
	if (gc == NULL)
		return (NULL);
	gc_add_back(head, gc);
	return (ptr);
}
