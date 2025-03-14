/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/14 01:43:51 by kkoujan          ###   ########.fr       */
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

	while (**cmd && **cmd != end_char)
	{
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

int	var_quote_token(char **str, t_token **head)
{
	int		j;
	char	*token;
	char	buffer[256];
	t_token	*node_token;
	int		is_var;

	j = 0;
	is_var = 0;
	while (**str)
	{
		if (**str == '$')
		{
			if ((*(*str + 1) == '\0' || is_whitespace(*(*str + 1)) || \
			*(*str + 1) == '"'))
			{
				buffer[j] = **str;
				j++;
				(*str)++;
				continue ;
			}
			buffer[j] = 0;
			if (j > 0)
			{
				token = ft_strdup(buffer);
				if (!token)
					return (free(token), 0);
				if (is_var)
					node_token = init_token(VAR_T, token);
				else
					node_token = init_token(ARG_T, token);
				if (!node_token)
					return (free(token), 0);
				ft_token_add_back(head, node_token);
				j = 0;
				is_var = 1;
			}
		}
		else if (**str == '"')
		{
			buffer[j] = 0;
			token = ft_strdup(buffer);
			if (!token)
				return (free(token), 0);
			if (is_var)
				node_token = init_token(VAR_T, token);
			else
				node_token = init_token(ARG_T, token);
			if (!node_token)
				return (free(token), 0);
			ft_token_add_back(head, node_token);
			j = 0;
			(*str)++;
			return (1);
		}
		else if (is_whitespace(**str) && is_var)
		{
			buffer[j] = 0;
			token = ft_strdup(buffer);
			if (!token)
				return (free(token), 0);
			node_token = init_token(VAR_T, token);
			is_var = 0;
			if (!node_token)
				return (free(token), 0);
			ft_token_add_back(head, node_token);
			j = 0;
		}
		else
		{
			buffer[j] = **str;
			j++;
		}
		(*str)++;
	}
	return (1);
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
			if (!var_quote_token(&current_pos, &head))
				return (NULL);
			i = current_pos - cmd;
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