/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:02:08 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/30 10:15:06 by kkoujan          ###   ########.fr       */
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
	return (gc);
}

void	ft_gctadd_back(t_gc **head, t_gc *new)
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

// void	add_to_gc(t_gc **head, void	*st, int type)
// {
// 	t_gc	*gc;

// 	if (type == 0)
// 	{
// 		st = (t_token*)st;
		
// 	}
// }

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
	ft_gcadd_back(head, gc);
	return (ptr);
}
