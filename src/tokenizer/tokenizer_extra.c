/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:07:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/29 20:55:24 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"

int	str_concat_len(char *cmd)
{
	int		i;
	int		counter;
	char	quote_char;
	int		in_quote;

	counter = 0;
	i = -1;
	in_quote = 0;
	quote_char = 0;
	while (cmd[++i])
	{
		if (!in_quote && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			in_quote = 1;
			quote_char = cmd[i];
		}
		else if (in_quote && cmd[i] == quote_char)
		{
			in_quote = 0;
			quote_char = 0;
		}
		else
			counter++;
	}
	return (counter);
}

char	*concat_str(char *cmd, char *str, int len)
{
	int		i;
	char	quote_char;
	int		in_quote;

	i = -1;
	in_quote = 0;
	quote_char = 0;
	len = 0;
	while (cmd[++i])
	{
		if (!in_quote && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			in_quote = 1;
			quote_char = cmd[i];
		}
		else if (in_quote && cmd[i] == quote_char)
		{
			in_quote = 0;
			quote_char = 0;
		}
		else
			str[len++] = cmd[i];
	}
	str[len] = '\0';
	return (str);
}

char	*remove_quotes(char *cmd)
{
	int		len;
	char	*str;

	len = str_concat_len(cmd);
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str = concat_str(cmd, str, len);
	if (str == NULL)
		return (NULL);
	return (str);
}

char	*process_word_tokens(t_token *lst, t_token **next_ptr, int *in_qt)
{
	t_token	*curr;
	char	*val;
	t_token	*prev;

	curr = lst->next;
	val = ft_strdup(lst->val);
	if (!val)
		return (NULL);
	while (curr && (curr->type == WORD_T || curr->type == QT_T))
	{
		if (curr->type == QT_T || (curr->type == WORD_T && curr->v_in_qt))
			*in_qt = 1;
		if (curr && curr->type == QT_T)
		{
			prev = curr;
			curr = curr->next;
			free(prev);
			continue ;
		}
		val = join_token(&curr, val);
		if (!val)
			return (NULL);
	}
	return (*next_ptr = curr, val);
}

void	handle_word_token(t_token *lst, t_token **next_ptr)
{
	char	*val;
	int		in_qt;

	in_qt = lst->v_in_qt;
	if (lst->v_in_qt == 1)
		in_qt = 1;
	val = process_word_tokens(lst, next_ptr, &in_qt);
	if (!val)
		return ;
	free(lst->val);
	lst->val = val;
	lst->v_in_qt = in_qt;
	lst->next = *next_ptr;
}
