/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_double_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:59:35 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/21 09:30:48 by kkoujan          ###   ########.fr       */
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

int	loop_double_quote(char **cmd_ptr, t_token **head, char **start, int *len)
{
	char	*cmd;
	int		var;

	cmd = *cmd_ptr + 1;
	*start = *start + 1;
	while (cmd[*len] && cmd[*len] != '\"')
	{
		if (cmd[*len] == '$' && (is_var_spchar(cmd[*len + 1]) || \
		ft_isalpha(cmd[*len + 1])))
		{
			if (*len > 0 && add_token(head, *start, *len, WORD_T) == 0)
				return (-1);
			var = handle_var(cmd + *len, head);
			if (var < 0)
				return (-1);
			cmd = cmd + *len + var;
			*start = cmd;
			*len = 0;
		}
		else
			(*len)++;
	}
	return (1);
}

int	handle_double_quote(char *cmd, t_token **head)
{
	int		len;
	char	*start;
	int		offset;

	ft_token_add_back(head, init_token(QT_T, NULL));
	len = 0;
	offset = double_quote_len(cmd);
	start = cmd;
	if (offset == 1)
	{
		if (add_token(head, start + 1, 0, WORD_T) == 0)
			return (-1);
		return (2);
	}
	if (loop_double_quote(&cmd, head, &start, &len) == -1)
		return (-1);
	if (offset == len)
		len--;
	if (len > 0 && add_token(head, start, len, WORD_T) == 0)
		return (-1);
	return (offset + 1);
}
