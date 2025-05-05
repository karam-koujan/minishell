/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:19:41 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/05 12:02:03 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	loop_over_var(t_token **prev, t_token **curr, t_token **next, \
	char **arr)
{
	*prev = (*curr)->next;
	*next = init_token(WORD_T, ft_strdup(*arr));
	if (!(*next))
		return ;
	(*curr)->next = *next;
	*curr = *next;
	if (*(arr + 1) != NULL)
	{
		*next = init_token(SP_T, NULL);
		if (!(*next))
			return ;
		(*curr)->next = *next;
		*curr = *next;
	}
	(*curr)->next = *prev;
}

void	insert_var(t_token **tokenlst, char **arr)
{
	int		i;
	t_token	*next;
	t_token	*curr;
	t_token	*prev;

	curr = *tokenlst;
	next = NULL;
	prev = NULL;
	i = 0;
	curr->type = WORD_T;
	free(curr->val);
	curr->val = ft_strdup(arr[0]);
	if (!curr->val)
		return ;
	if (arr[1] == NULL)
		return ;
	next = init_token(SP_T, NULL);
	if (!next)
		return ;
	next->next = curr->next;
	curr->next = next;
	curr = next;
	while (arr[++i])
		loop_over_var(&prev, &curr, &next, arr + i);
}

t_token	*handle_expand_var(t_token *tokenlst, t_env *env, int in_quote)
{
	char	*val;
	char	**arr;
	t_token	*curr;

	curr = tokenlst;
	if (!curr)
		return (NULL);
	val = ft_getenv_val(env, curr->val);
	if (in_quote || ft_strlen(val) == 0)
	{
		free(tokenlst->val);
		tokenlst->type = WORD_T;
		tokenlst->val = val;
		return (tokenlst->next);
	}
	arr = ft_split(val, ' ');
	if (arr == NULL)
		return (NULL);
	if (curr)
		insert_var(&curr, arr);
	return (free_arr(arr), tokenlst->next);
}

void	join_var(t_token **tokenlst, t_env *env)
{
	t_token	*lst;
	int		in_quote;
	int     in_redir;

	lst = *tokenlst;
	in_quote = 0;
	in_redir = 0;
	while (lst)
	{
		if (lst->type == QT_T)
		{
			in_quote = 1;
			lst = lst->next;
			continue ;
		}
		if (lst->type == VAR_T)
		{
			if (!in_redir)
				lst = handle_expand_var(lst, env, in_quote);
			else
				lst = lst->next;
			in_redir = 0;
			continue ;
		}
		if (in_quote && lst->type == SP_T)
			in_quote = 0;
		if (lst->type == REDIR_APPEND || lst->type == REDIR_B_T \
				|| lst->type == REDIR_F_T)
			in_redir = 1;
		else if (lst->type != SP_T && in_redir)
			in_redir = 0;
		lst = lst->next;
	}
}
