/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/20 01:17:13 by kkoujan          ###   ########.fr       */
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

char	*var_token(char **cmd)
{
	int		count;
	char	*arg;
	char	*start;

	count = 0;
	start = *cmd;
	while (**cmd && ft_isalnum(**cmd))
	{
		(*cmd)++;
		count++;
	}
	if (**cmd == '$')
		(*cmd)--;
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



int	var_quote_token(char **str, t_token **head)
{
	int		len;
	int		is_var;
	char	*start_pos;

	len = 0;
	is_var = 0;
	start_pos = *str;
	while (**str)
	{
		if (**str == '$' && *(*str + 1) == '$')
		{
			if (len > 0)
			{
				if ((is_var && add_token(head, start_pos, len, VAR_T) == 0) || \
					(!is_var && add_token(head, start_pos, len, ARG_T) == 0))
					return (0);
			}
			(*str)++;
			if (add_token(head, *str, 1, VAR_T) == 0)
				return (0);
			(*str)++;
			start_pos = *str;
			len = 0;
			is_var = 0;
			continue ;
		}
		else if (**str == '$' && !ft_isalnum(*(*str + 1)))
			len++;
		else if (**str == '$' && ft_isalnum(*(*str + 1)))
		{
			is_var = 1;
			if (add_token(head, start_pos, len, ARG_T) == 0)
				return (0);
			start_pos = *str + 1;
			len = 0;
		}
		else if (**str == '"' && !is_escaped(*str) && len > 0)
		{
			if (is_var)
				return (add_token(head, start_pos, len, VAR_T));
			return (add_token(head, start_pos, len, ARG_T));
		}
		else if (is_whitespace(**str) && is_var)
		{
			add_token(head, start_pos, len, VAR_T);
			len = 1;
			is_var = 0;
			start_pos = *str;
		}
		else
			len++;
		(*str)++;
	}
	return (1);
}

int var_tokenization(char **str, t_token **head)
{
	char	*token;
	t_token	*node_token;

	if (**str == '$')
	{
		token = ft_strdup("$");
		if (!token)
			return (free(token), 0);
		node_token = init_token(VAR_T, token);
		if (!node_token)
			return (free(token), 0);
		return (ft_token_add_back((t_token **)head, (t_token *)node_token), 1);
	}
	else if (**str == '?')
	{
		token = ft_strdup("?");
		if (!token)
			return (free(token), 0);
		node_token = init_token(VAR_T, token);
		if (!node_token)
			return (free(token), 0);
		return (ft_token_add_back((t_token **)head, (t_token *)node_token), 1);
	}
	else if (ft_isalnum(**str) || **str == '_')
	{
		token = var_token(str);
		if (!token)
			return (free(token), 0);
		node_token = init_token(VAR_T, token);
		if (!node_token)
			return (free(token), 0);
		return (ft_token_add_back((t_token **)head, (t_token *)node_token), 1);
	}
	return (0);
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

int	handle_double_quote(char *cmd, t_token **head)
{
	int		len;
	char	*start;
	int		var;
	int		offset;

	len = 1;
	offset = 1;
	start = cmd + 1;
	while (cmd[offset] && (cmd[offset] != '\"' || \
		(cmd[offset] == '\"' && is_escaped(cmd + offset))))
		offset++;
	while (cmd[len] && (cmd[len] != '\"' || \
		(cmd[len] == '\"' && is_escaped(cmd + len))))
	{
		printf("start : %s\n", cmd + len);
		if (cmd[len] == '$' && (is_var_spchar(cmd[len + 1]) || \
		ft_isalpha(cmd[len + 1])))
		{
			if (len > 1 && add_token(head, start, len, ARG_T) == 0)
				return (-1);
			var = handle_var(cmd + len, head);
			printf("s : %s varidx %i len : %i\n", cmd + len,var, len);
			if (var < 0)
				return (-1);
			cmd = cmd + len + var;
			start = cmd;
			len = 0;
		}
		else
			len++;
	}
	if (start < cmd + len && add_token(head, start, len, ARG_T) == 0)
		return (-1);
	return (offset);
}

t_token	*tokenize(char *cmd)
{
	//char	*token;
	t_token	*head;
	int		idx;
	head = NULL;

	// t_token	*node_token;
	// char	*start_pos;
	// int		len;
	// int		is_cmd;
	// len = 0;

	// start_pos = cmd;
	// head = NULL;
	// is_cmd = 1;
	idx = 0;
	while (*cmd)
	{
		if (*cmd == '$' && (is_var_spchar(cmd[1]) || ft_isalpha(cmd[1])))
		{
			idx = handle_var(cmd, &head);
			if (idx < 0)
				return (NULL);
			cmd = cmd + idx;
			continue ;
		}
		if (*cmd == '|' || *cmd == '>' || *cmd == '<')
		{
			idx = handle_operation(cmd, &head);
			if (idx < 0)
				return (NULL);
			cmd = cmd + idx;
			continue ;
		}
		if (*cmd == '\'' && !is_escaped(cmd))
		{
			idx = handle_single_quote(cmd, &head);
			if (idx < 0)
				return (NULL);
			cmd = cmd + idx;
			continue ;
		}
		if (*cmd == '"' && !is_escaped(cmd))
		{
			idx = handle_double_quote(cmd, &head);
			if (idx < 0)
				return (NULL);
			cmd = cmd + idx;
			continue ;
		}
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
