/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:07:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/31 03:08:21 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"

char	*join_cmd(char *cmd)
{
	int	i;
	int	counter;
	char	*str;
	char	quote_char;
	int	in_quote;

	counter = 0;
	i = 0;
	in_quote = 0;
	quote_char = 0;	
	while (cmd[i])
	{
		if (!in_quote && (cmd[i] == '\'' || cmd[i] == '\"'))
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
		i++;
	}
	str = malloc((counter + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	counter = 0;
	i = 0;
	in_quote = 0;
	quote_char = 0;
	
	while (cmd[i])
	{
		if (!in_quote && (cmd[i] == '\'' || cmd[i] == '\"'))
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
			str[counter++] = cmd[i];
		i++;
	}
	str[counter] = '\0';
	return (str);
}

t_token	*handle_tokenizer(t_token **tokenlst)
{
	t_token *lst;
	char	*val;

	lst = *tokenlst;
	while (lst)
	{
		if (lst->type == WORD_T)
		{
			val = join_cmd(lst->val);
			if (val == NULL)
				return (NULL);
			free(lst->val);
			lst->val = val;
			val = NULL;
		}
		lst = lst->next;
	}
	return *tokenlst;
}