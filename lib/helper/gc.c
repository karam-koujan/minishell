/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:15:54 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/30 10:02:50 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	free_in_exit(t_token *token_head, t_cmd_table *cmd_table, \
	t_env *env)
{
	ft_token_lstclear(&token_head, free);
	free_table(cmd_table);
	free_env(&env, free);
}
