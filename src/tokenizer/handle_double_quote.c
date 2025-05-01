/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_double_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:59:35 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/01 15:22:21 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"

int	double_quote_len(char *cmd)
{
	int		offset;

	offset = 1;
	while (cmd[offset] && cmd[offset] != '\"')
		offset++;

	return (offset);
}

int	loop_double_quote(char **cmd_ptr, t_token **head, t_double_quote *st, \
	int *in_herdoc)
{
	char	*cmd;
	int		var;

	cmd = *cmd_ptr + 1;
	st->start = st->start + 1;
	while (cmd[st->len] && cmd[st->len] != '\"')
	{
		if ((cmd[st->len] == '$' && !(*in_herdoc)) && \
		(is_var_spchar(cmd[st->len + 1]) || ft_isalpha(cmd[st->len + 1])))
		{
			if (st->len > 0 && add_token(head, st->start, st->len, WORD_T) == 0)
				return (-1);
			var = handle_var(cmd + st->len, head);
			if (var < 0)
				return (-1);
			cmd = cmd + st->len + var;
			st->start = cmd;
			st->len = 0;
		}
		else
			(st->len)++;
	}
	return (1);
}

int	handle_double_quote(char *cmd, t_token **head, int *in_herdoc)
{
	int				offset;
	t_double_quote	st;

	ft_token_add_back(head, init_token(QT_T, NULL));
	st.len = 0;
	offset = double_quote_len(cmd);
	st.start = cmd;
	if (offset == 1)
	{
		if (add_token(head, st.start + 1, 0, WORD_T) == 0)
			return (-1);
		return (2);
	}
	if (loop_double_quote(&cmd, head, &st, in_herdoc) == -1)
		return (-1);
	if (offset == st.len)
		st.len--;
	if (st.len > 0 && add_token(head, st.start, st.len, WORD_T) == 0)
		return (-1);
	return (offset + 1);
}
