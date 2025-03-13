/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/13 04:59:00 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"

char	*quote_token(char **cmd, char end_char)
{
	int		count;
	char	*arg;
	char	*start;

	count = 0;
	start = *cmd;
	if (**cmd == '$')
		return (NULL);
	while (**cmd && **cmd != end_char)
	{
		if (end_char == '"' && **cmd == '$')
			break ;
		(*cmd)++;
		count++;
	}
	if (**cmd == end_char)
		(*cmd)++;
	arg = malloc((count + 1) * sizeof(char));
	if (!arg)
		return (NULL);
	ft_memcpy(arg, start, count);
	arg[count] = 0;
	return (arg);
}

int	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char	*var_token(char **cmd)
{
	int		count;
	char	*arg;
	char	*start;

	count = 0;
	start = *cmd;
	while (**cmd && **cmd != '"' && !is_whitespace(**cmd))
	{
		(*cmd)++;
		count++;
	}
	if (**cmd == '"')
		(*cmd)++;
	arg = malloc((count + 1) * sizeof(char));
	if (!arg)
		return (NULL);
	ft_memcpy(arg, start, count);
	arg[count] = 0;
	return (arg);
}


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

t_token	*tokenize(char *cmd)
{
	int		i;
	char	*token;
	t_token	*head;
	t_token	*node_token;
	char	*current_pos;

	i = 0;
	head = NULL;
	while (cmd[i])
	{
		if (cmd[i] == '\'')
		{
			current_pos = cmd + i + 1;
			token = quote_token(&current_pos, '\'');
			if (!token)
				return (free(token), NULL);
			node_token = init_token(ARG_T, token);

			if (!node_token)
				return (free(token), NULL);
			ft_token_add_back((t_token **)&head, (t_token *)node_token);
			i = current_pos - cmd;
		}
		if (cmd[i] == '"')
		{
			current_pos = cmd + i + 1;
			token = quote_token(&current_pos, '"');
			if (!token)
				free(token);
			if (token)
			{
				node_token = init_token(ARG_T, token);
				if (!node_token)
					free(token);
			}
			ft_token_add_back((t_token **)&head, (t_token *)node_token);
			i = current_pos - cmd;
			if (cmd[i] == '$')
			{
				current_pos = cmd + i + 1;
				token = var_token(&current_pos);
				if (!token)
					return (free(token), NULL);
				node_token = init_token(VAR_T, token);
				if (!node_token)
					return (free(token), NULL);
				ft_token_add_back(&head, node_token);
				i = current_pos - cmd;
			}
		}
		if (cmd[i] == '$')
		{
			current_pos = cmd + i + 1;
			token = var_token(&current_pos);
			if (!token)
				return (free(token), NULL);
			node_token = init_token(VAR_T, token);
			if (!node_token)
				return (free(token), NULL);
			ft_token_add_back(&head, node_token);
			i = current_pos - cmd;
		}
		if (cmd[i] == '|')
		{
			token = ft_strdup("|");
			if (!token)
				return (free(token), NULL);
			node_token = init_token(PIPE_T, token);
			if (!node_token)
				return (free(token), NULL);
			ft_token_add_back(&head, node_token);
			i++;
		}
		else
		{
			i++;
		}
	}
	return (head);
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