/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:14:14 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/21 22:58:38 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"

int	is_var_spchar(char c)
{
	if (c == '$' || c == '?' || c == '#' || c == '@' || \
			c == '*' || c == '!' || c == '-' || c == '_')
		return (1);
	return (0);
}

int	handle_word(char *cmd, t_token **head)
{
	int		len;
	char	*start;
	int		offset;

	len = 0;
	start = cmd;
	offset = 0;
	while (cmd[offset] && !is_whitespace(cmd[offset]) && cmd[offset] != '$')
		offset++;
	while (cmd[len++])
	{
		if (is_whitespace(cmd[len]) || cmd[len] == '$')
			break ;
	}
	if (len == 1)
		len++;
	if (add_token(head, start, len, WORD_T) == 0)
		return (-1);
	return (offset);
}

int	handle_whitespace(char *cmd, t_token **head)
{
	int		len;
	char	*start;
	int		offset;

	len = 0;
	start = cmd;
	offset = 0;
	while (cmd[offset] && is_whitespace(cmd[offset]))
		offset++;
	while (cmd[len++])
	{
		if (!is_whitespace(cmd[len]))
			break ;
	}
	if (add_token(head, start, len, SP_T) == 0)
		return (-1);
	return (offset);
}

void print_token_list(t_token *head)
{
    t_token *current = head;
    int count = 0;
    if (!head)
    {
        printf("Empty token list\n");
        return;
    }
    printf("TOKEN LIST:\n");
    while (current)
    {
        printf("Token %d:\n", count++);
        printf("  Type: %d\n", current->type);
        printf("  Value: %s\n", current->val ? current->val : "(null)");
        current = current->next;
    }
    printf("End of token list. Total tokens: %d\n", count);
}
