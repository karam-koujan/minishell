/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:15:54 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/19 12:02:18 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// t_gc	*init_addr(void	*addr)
// {
// 	t_gc	*gc;

// 	gc = malloc(sizeof(t_gc));
// 	if (gc == NULL)
// 		return (NULL);
// 	gc->addr = addr;
// 	gc->next = NULL;
// 	return (gc);
// }

// void	gc_add_back(t_gc **head, t_gc *new)
// {
// 	t_gc	*l;

// 	if (!head || !new)
// 		return ;
// 	if (*head == NULL)
// 	{
// 		*head = new;
// 	}
// 	else
// 	{
// 		l = *head;
// 		while (l && l->next != NULL)
// 		{
// 			l = l->next;
// 		}
// 		l->next = new;
// 	}
// }

// void	free_all(t_gc **head)
// {
// 	t_gc	*l;
// 	t_gc	*tmp;

// 	if (!head)
// 		return ;
// 	if (!(*head))
// 		return ;
// 	l = *head;
// 	tmp = NULL;
// 	while (l != NULL)
// 	{
// 		free(l->addr);
// 		tmp = l;
// 		l = l->next;
// 		free(tmp);
// 		tmp = NULL;
// 	}
// }

// void	*ft_malloc(void *ptr, t_gc **head, int enable_free)
// {
// 	t_gc	*gc;

// 	if (enable_free)
// 		return (free_all(head), NULL);
// 	if (ptr == NULL)
// 		return (NULL);
// 	gc = init_addr(ptr);
// 	if (gc == NULL)
// 		return (NULL);
// 	gc_add_back(head, gc);
// 	return (ptr);
// }

t_gc	*init_gc(void)
{
	t_gc	*gc;

	gc = malloc(sizeof(t_gc));
	if (!gc)
		return (NULL);
	gc->env = NULL;
	gc->parse = NULL;
	gc->tokenizer = NULL;
	return (gc);
}

void	add_to_gc(t_gc **gc, t_cmd_table *parse, \
					t_token *tokenizer, t_env *env)
{
	t_gc	*curr;

	if (!gc)
		return ;
	curr = *gc;
	if (!curr)
		return ;
	if (parse)
		curr->parse = parse;
	if (tokenizer)
		curr->tokenizer = tokenizer;
	if (env)
		curr->env = env;
}

void	free_all(t_gc **head)
{
	t_gc	*curr;

	if (!head)
		return ;
	curr = *head;
	if (!curr)
		return ;
	if (curr->parse)
		free_table(curr->parse);
	if (curr->tokenizer)
		ft_token_lstclear(&curr->tokenizer, free);
	if (curr->env)
		free_env(&curr->env, free);
	free(curr);
	*head = NULL;
}

void	clear_parsing(t_gc **head)
{
	t_gc	*curr;

	if (!head)
		return ;
	curr = *head;
	if (!curr)
		return ;
	if (curr->parse)
	{
		free_table(curr->parse);
		curr->parse = NULL;
	}
	if (curr->tokenizer)
	{
		ft_token_lstclear(&curr->tokenizer, free);
		curr->tokenizer = NULL;
	}
}
