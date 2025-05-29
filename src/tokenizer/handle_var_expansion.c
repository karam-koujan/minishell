/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:19:41 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/29 18:57:32 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	loop_over_var(t_token **prev, t_token **curr, t_token **next, \
	char **arr)
{
	*prev = (*curr)->next;
	*next = init_token(WORD_T, ft_strdup(*arr), 0);
	if (!(*next))
		return ;
	(*curr)->next = *next;
	*curr = *next;
	if (*(arr + 1) != NULL)
	{
		*next = init_token(SP_T, NULL, 0);
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
	next = init_token(SP_T, NULL, 0);
	if (!next)
		return ;
	next->next = curr->next;
	curr->next = next;
	curr = next;
	while (arr[++i])
		loop_over_var(&prev, &curr, &next, arr + i);
}

t_token	*handle_empty_var(t_token **tokenlst, char *val)
{
	t_token	*curr;

	if (!(*tokenlst)->next)
	{
		free((*tokenlst)->val);
		free(*tokenlst);
		return (NULL);
	}
	free((*tokenlst)->val);
	(*tokenlst)->val = (*tokenlst)->next->val;
	(*tokenlst)->type = (*tokenlst)->next->type;
	(*tokenlst)->v_in_qt = (*tokenlst)->next->v_in_qt;
	curr = (*tokenlst)->next;
	(*tokenlst)->next = curr->next;
	free(curr);
	free(val);
	return (*tokenlst);
}

t_token	*handle_expand_var(t_token *tokenlst, t_env *env)
{
	char	*val;
	char	**arr;
	t_token	*curr;

	curr = tokenlst;
	if (!curr)
		return (NULL);
	val = ft_getenv_val(env, curr->val);
	if (tokenlst->v_in_qt)
	{
		free(tokenlst->val);
		tokenlst->type = WORD_T;
		tokenlst->val = val;
		return (tokenlst->next);
	}
	else if (!tokenlst->v_in_qt && ft_strlen(val) == 0)
		return (handle_empty_var(&tokenlst, val));
	arr = ft_split(val, ' ');
	if (arr == NULL)
		return (free(val), NULL);
	if (curr)
		insert_var(&curr, arr);
	return (free(val), free_arr(arr), tokenlst->next);
}

void	join_var(t_token **tokenlst, t_env *env)
{
	t_token	*lst;
	int		in_redir;

	lst = *tokenlst;
	in_redir = 0;
	while (lst)
	{
		if (lst->type == VAR_T)
		{
			if (!in_redir)
				lst = handle_expand_var(lst, env);
			else
				lst = lst->next;
			if (lst && lst->type == SP_T)
				in_redir = 0;
			continue ;
		}
		if (lst->type == WORD_T && lst->next && lst->next->type == SP_T)
			in_redir = 0;
		if (lst->type == APPEND_T || lst->type == REDIR_B_T \
				|| lst->type == REDIR_F_T)
			in_redir = 1;
		lst = lst->next;
	}
}
