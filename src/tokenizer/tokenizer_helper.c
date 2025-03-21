/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:14:14 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/21 20:15:15 by kkoujan          ###   ########.fr       */
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
