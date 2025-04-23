/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:07:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/23 15:34:58 by kkoujan          ###   ########.fr       */
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

char	*process_word_tokens(t_token *lst, t_token **next_ptr)
{
	t_token	*curr;
	t_token	*next_node;
	char	*val;
	char	*tmp;

	curr = lst->next;
	val = ft_strdup(lst->val);
	if (!val)
		return (NULL);
	while (curr && (curr->type == WORD_T || curr->type == QT_T))
	{
		if (curr && curr->type == QT_T)
		{
			curr = curr->next;
			continue ;
		}
		tmp = val;
		val = ft_strjoin(tmp, curr->val);
		free(tmp);
		if (!val)
			return (NULL);
		next_node = curr;
		curr = curr->next;
		free(next_node->val);
		free(next_node);
	}
	return (*next_ptr = curr, val);
}

void	handle_word_token(t_token *lst, t_token **next_ptr)
{
	char	*val;

	val = process_word_tokens(lst, next_ptr);
	if (!val)
		return ;
	free(lst->val);
	lst->val = val;
	lst->next = *next_ptr;
}
