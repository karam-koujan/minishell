/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:28:03 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/16 14:08:05 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/error_checker.h"

int	has_unclosed_quotes(char *cmd)
{
	int	is_double_quote;
	int	is_single_quote;
	int	i;

	i = -1;
	is_double_quote = 0;
	is_single_quote = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' && is_double_quote == 0)
			is_single_quote = !is_single_quote;
		else if (cmd[i] == '"' && is_single_quote == 0)
			is_double_quote = !is_double_quote;
	}
	return ((is_double_quote || is_single_quote));
}

int	has_misplaced_pipes(char *cmd)
{
	int	i;
	int	pipe;

	pipe = 0;
	if (check_edges(cmd, "|", 0) > 0)
		return (check_edges(cmd, "|", 0));
	i = -check_edges(cmd, "|", 0) - 1;
	while (cmd[++i])
	{
		if (!in_quotes(cmd, i))
		{
			if (cmd[i] == '|' && pipe == 1)
				return ((int)cmd[i]);
			if (cmd[i] == '|' && pipe == 0)
				pipe = 1;
			// if (ft_strchr("<>", cmd[i]) != NULL && pipe != 0)
			// 	return ((int)cmd[i]);
			if (cmd[i] != '|' && !is_whitespace(cmd[i]))
				pipe = 0;
		}
	}
	return (0);
}


int	has_invalid_redir(char *cmd)
{
	int	i;
	int	redirfor;
	int	redirback;

	redirfor = 0;
	redirback = 0;
	if (check_edges(cmd, "<>", 2) > 0)
		return (check_edges(cmd, "<>", 2));
	i = -check_edges(cmd, "<>", 2) - 1;
	while (cmd[++i])
	{
		if (!in_quotes(cmd, i))
		{
			if (detect_invalid_redir(cmd, &redirfor, &redirback, i) != 0)
				return (detect_invalid_redir(cmd, &redirfor, &redirback, i));
			increment_redir(cmd[i], &redirfor, &redirback);
			if ((cmd[i] != '<' && cmd[i] != '>') && !is_whitespace(cmd[i]) && \
			ft_strchr("|", cmd[i]) == NULL)
			{
				redirback = 0;
				redirfor = 0;
			}
		}
	}
	return (0);
}


int	has_logical_op(char	*cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		if (!in_quotes(cmd,i))
		{
			if (ft_strnstr(cmd + i, "&&", 2))
				return ((int)*ft_strnstr(cmd + i, "&&", ft_strlen(cmd + i)));
			if (ft_strnstr(cmd + i, "||", 2))
				return ((int)*ft_strnstr(cmd + i, "||", ft_strlen(cmd + i)));
		}
		i++;
	}
	return (0);
}
int	index_of(char *str, char c, int len)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c && len > 0)
		{
			len--;
			i++;
			if (len == 0)
				return (i);
		}
		i++;
	}
	return (-1);
}
#include <stdio.h>

int	err_idx_chr(char	*cmd)
{
	int		idx;
	char	c;

	idx = -1;
	c = has_unclosed_quotes(cmd);
	if (c > 0 && (idx > index_of(cmd, c, 1) || idx == -1))
		idx = index_of(cmd, c, 1);
	c = has_invalid_redir(cmd);
	if (c > 0 && (idx > index_of(cmd, c, 1) || idx == -1))
		idx = index_of(cmd, c, 1);
	c = has_misplaced_pipes(cmd);
	if (c > 0 && (idx > index_of(cmd, c, 1) || idx == -1))
		idx = index_of(cmd, c, 1);
	c = has_logical_op(cmd);
	if (c > 0 && (idx > index_of(cmd, c, 2) || idx == -1))
	{
		idx = index_of(cmd, c, 2);
		printf("idx is here: %i\n", idx);
	}
	return (idx);
}

int	syntax_error(char	*cmd)
{
	char	*unclosed_quote_err;
	char	*log_op_err;
	char	*operator_err;
	int		operator;
	int		err_idx;

	unclosed_quote_err = "minishell: syntax error: unclosed quote\n";
	operator_err = "minishell: syntax error near unexpected token ";
	log_op_err = "minishell: syntax error : unsupported operator ";
	err_idx = err_idx_chr(cmd);
	printf("idx:%i\n", err_idx_chr(cmd));
	operator = has_invalid_redir(cmd);
	if (has_unclosed_quotes(cmd))
		return (write(2, unclosed_quote_err, ft_strlen(unclosed_quote_err)), 0);
	if (operator && write(2, operator_err, ft_strlen(operator_err)))
		return (write(2, "`", 1), write(2, &operator, 1), write(2, "'\n", 2), 0);
	operator = has_misplaced_pipes(cmd);
	if (operator && write(2, operator_err, ft_strlen(operator_err)))
		return (write(2, "`", 1), write(2, &operator, 1), write(2, "'\n", 2), 0);
	operator = has_logical_op(cmd);
	if (operator && write(2, log_op_err, ft_strlen(log_op_err)))
		return (write(2, "`", 1), write(2, &operator, 1), \
		write(2, &operator, 1), write(2, "'\n", 2), 0);
	return (1);
}
