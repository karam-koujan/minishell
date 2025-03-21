/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/21 19:52:31 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"

int	is_escaped(char *str)
{
	int	count;

	count = 0;
	str = str + 1;
	while (*str && *str == '\\')
	{
		str = str + 1;
		count++;
	}
	return (count % 2 != 0);
}

int	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
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

int	add_token(t_token **head, char *start_ptr, int len, t_token_type type)
{
	char	*token;
	t_token	*node_token;

	token = ft_substr(start_ptr, 0, len);
	if (!token)
		return (free(token), 0);
	node_token = init_token(type, token);
	if (!node_token)
		return (free(token), 0);
	ft_token_add_back(head, node_token);
	return (1);
}





int	handle_var(char *cmd, t_token **head)
{
	int		i;
	char	*start;

	i = 0;
	start = cmd + 1;
	while (cmd[++i])
	{
		if (cmd[i] == '_')
			continue ;
		if (is_whitespace(cmd[i]) || !ft_isalnum(cmd[i]) \
			|| (cmd[i] == '$' && i > 1))
			break ;
	}
	if (i == 1)
		i++;
	if (add_token(head, start, i - 1, VAR_T) == 0)
		return (-1);
	return (i);
}

int	handle_operation(char *cmd, t_token **head)
{
	t_token	*node_token;
	int		idx;

	node_token = NULL;
	idx = 1;
	if (*cmd == '|')
		node_token = init_token(PIPE_T, NULL);
	else if (*cmd == '>' && ft_strncmp(cmd, ">>", 2) != 0)
		node_token = init_token(REDIR_F_T, NULL);
	else if (*cmd == '<' && ft_strncmp(cmd, "<<", 2) != 0)
		node_token = init_token(REDIR_B_T, NULL);
	else if (*cmd == '>' && ft_strncmp(cmd, ">>", 2) == 0 && (++idx))
		node_token = init_token(APPEND_T, NULL);
	else if (*cmd == '<' && ft_strncmp(cmd, "<<", 2) == 0 && (++idx))
		node_token = init_token(HERDOC_T, NULL);
	if (!node_token)
		return (-1);
	ft_token_add_back(head, node_token);
	return (idx);
}

int	handle_single_quote(char *cmd, t_token **head)
{
	int		len;
	char	*start;

	len = 1;
	start = cmd + 1;

	while (cmd[len] && (cmd[len] != '\'' || \
		(cmd[len] == '\'' && is_escaped(cmd + len))))
		len++;
	if (add_token(head, start, len - 1, ARG_T) == 0)
		return (-1);
	return (len + 1);
}

int	is_var_spchar(char c)
{
	if (c == '$' || c == '?' || c == '#' || c == '@' || \
			c == '*' || c == '!' || c == '-' || c == '_')
		return (1);
	return (0);
}

int	double_quote_len(char *cmd)
{
	int		offset;
	char	*start;

	offset = 1;
	start = cmd + 1;
	while (cmd[offset] && (cmd[offset] != '\"' || \
		(cmd[offset] == '\"' && is_escaped(cmd + offset))))
		offset++;

	return (offset);
}

int	loop_double_quote(char **cmd_ptr,t_token **head,char **start,int *len)
{
	char	*cmd;
	int		var;

	cmd = *cmd_ptr + 1;
	*start = *start + 1;
	while (cmd[*len] && (cmd[*len] != '\"' || \
		(cmd[*len] == '\"' && is_escaped(cmd + *len))))
	{
		if (cmd[*len] == '$' && (is_var_spchar(cmd[*len + 1]) || \
		ft_isalpha(cmd[*len + 1])))
		{
			printf("start: %s  len: %i\n", *start, *len);
			if (*len > 0 && add_token(head, *start, *len, ARG_T) == 0)
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

	len = 0;
	offset = double_quote_len(cmd);
	start = cmd;
	if (offset == 1)
	{
		if (add_token(head, start + 1, 0, ARG_T) == 0)
			return (-1);
		return (2);
	}
	if (loop_double_quote(&cmd, head, &start, &len) == -1)
		return (-1);
	if (offset == len)
		len--;
	if (len > 0 && add_token(head, start, len, ARG_T) == 0)
		return (-1);
	return (offset + 1);
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
	if (add_token(head, start, len, ARG_T) == 0)
		return (-1);
	return (offset);
}

int	handle_cmd(char *cmd, t_token **head)
{
	if (*cmd == '$' && (is_var_spchar(cmd[1]) || ft_isalpha(cmd[1])))
		return (handle_var(cmd, head));
	if (*cmd == '|' || *cmd == '>' || *cmd == '<')
		return (handle_operation(cmd, head));
	if (*cmd == '\'' && !is_escaped(cmd))
		return (handle_single_quote(cmd, head));
	if (*cmd == '"' && !is_escaped(cmd))
		return (handle_double_quote(cmd, head));
	if (*cmd && !is_whitespace(*cmd))
		return (handle_word(cmd, head));
	return (0);
}

t_token	*tokenize(char *cmd)
{
	t_token	*head;
	int		idx;

	head = NULL;
	idx = 0;
	while (*cmd)
	{
		idx = handle_cmd(cmd, &head);
		if (idx < 0)
			return (NULL);
		else if (idx > 0)
			cmd = cmd + idx;
		else
			cmd++;
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
