/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:07:49 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/05 11:17:26 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


t_token	*init_token(t_token_type type, char *val)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->val = val;
	token->next = NULL;
	return (token);
}

void	ft_token_add_back(t_token **lst, t_token *new)
{
	t_token	*l;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		l = *lst;
		while (l->next != NULL)
		{
			l = l->next;
		}
		l->next = new;
	}
}

void	loop_over_var(t_token **prev, t_token **curr, t_token **next, \
	char **arr)
{
	*prev = (*curr)->next;
	*next = init_token(WORD_T, ft_strdup(*arr));
	if (!(*next))
		return ;
	(*curr)->next = *next;
	*curr = *next;
	if (*arr + 1 != NULL)
	{
		*next = init_token(SP_T, NULL);
		if (!(*next))
			return ;
		(*curr)->next = *next;
		*curr = *next;
	}
	(*curr)->next = *prev;
}

void	insert_var(t_token*tokenlst, char **arr)
{
	int		i;
	t_token	*next;
	t_token	*curr;
	t_token	*prev;

	curr = tokenlst;
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
		insert_var(curr, arr);
	return (free_arr(arr), tokenlst->next);
}

void	join_var(t_token **tokenlst, t_env *env)
{
	t_token	*lst;
	int		in_quote;

	lst = *tokenlst;
	in_quote = 0;
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
			lst = handle_expand_var(lst, env, in_quote);
			continue ;
		}
		else if (in_quote && lst->type == SP_T)
			in_quote = 0;
		lst = lst->next;
	}
}

t_token	*handle_tokenizer(t_token **tokenlst, t_env *env)
{
	join_var(tokenlst, env);
	join_cmd(tokenlst);
	return (*tokenlst);
}

void	ft_token_lstclear(t_token **lst, void (*del)(void*))
{
	t_token	*l;
	void	*tmp;

	if (!lst || !del)
		return ;
	if (*lst == NULL)
		return ;
	l = *lst;
	while (l)
	{
		del(l->val);
		tmp = l->next;
		free(l);
		l = tmp;
	}
	*lst = NULL;
}

int	add_token(t_token **head, char *start_ptr, int len, t_token_type type)
{
	char	*token;
	t_token	*node_token;

	token = ft_substr(start_ptr, 0, len);
	if (!token)
		return (free(token), 0);
	node_token = init_token(type, token);
	if (!node_token)
		return (free(token), 0);
	ft_token_add_back(head, node_token);
	return (1);
}
