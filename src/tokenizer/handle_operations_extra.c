/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operations_extra.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:19:19 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/26 17:41:29 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_vr_token(t_token **head, char *start_ptr, int len, \
	t_token_data *token_data)
{
	char	*token;
	t_token	*node_token;

	token = ft_substr(start_ptr, 0, len);
	if (!token)
		return (free(token), 0);
	node_token = init_token(token_data->type, token, token_data->v_in_qt);
	if (!node_token)
		return (free(token), 0);
	ft_token_add_back(head, node_token);
	return (1);
}

char	*join_token(t_token **curr, char *val)
{
	t_token	*next_node;
	char	*tmp;

	tmp = val;
	val = ft_strjoin(tmp, (*curr)->val);
	free(tmp);
	if (!val)
		return (NULL);
	next_node = *curr;
	*curr = (*curr)->next;
	free(next_node->val);
	free(next_node);
	return (val);
}
