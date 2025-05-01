/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/01 15:39:11 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"


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
		if (is_whitespace(cmd[i]) || !ft_isalnum(cmd[i]))
			break ;
	}
	if (i == 1)
		i++;
	if (add_token(head, start, i - 1, VAR_T) == 0)
		return (-1);
	return (i);
}

int	handle_operation(char *cmd, t_token **head, int *in_herdoc)
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
	{
		*in_herdoc = 1;
		node_token = init_token(HERDOC_T, NULL);
	}
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

	while (cmd[len] && cmd[len] != '\'')
		len++;
	if (add_token(head, start, len - 1, WORD_T) == 0)
		return (-1);
	return (len + 1);
}

int	handle_cmd(char *cmd, t_token **head, int *in_herdoc)
{
	if (!(*in_herdoc) && *cmd == '$' && (is_var_spchar(cmd[1]) \
	|| ft_isalpha(cmd[1])))
		return (handle_var(cmd, head));
	if (*cmd == '|' || *cmd == '>' || *cmd == '<')
		return (handle_operation(cmd, head, in_herdoc));
	if (*cmd == '\'')
		return (handle_single_quote(cmd, head));
	if (*cmd == '"' )
		return (handle_double_quote(cmd, head, in_herdoc));
	if (*cmd && !is_whitespace(*cmd))
		return (handle_word(cmd, head, in_herdoc));
	if (*cmd && is_whitespace(*cmd))
		return (handle_whitespace(cmd, head, in_herdoc));
	return (0);
}

t_token	*tokenize(char *cmd)
{
	t_token	*head;
	int		idx;
	int		in_herdoc;

	in_herdoc = 0;
	head = NULL;
	idx = 0;
	while (*cmd)
	{
		idx = handle_cmd(cmd, &head, &in_herdoc);
		if (idx < 0)
			return (NULL);
		else if (idx > 0)
			cmd = cmd + idx;
		else
			cmd++;
	}
	return (handle_tokenizer(&head));
	// return (head);
}
