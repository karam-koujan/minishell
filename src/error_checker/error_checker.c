/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:28:03 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/20 10:42:14 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/error_checker.h"

int	has_unclosed_quotes(char *cmd)
{
	int	is_double_quote;
	int	is_single_quote;
	int	i;
	int	sq;

	i = -1;
	sq = -1;
	is_double_quote = 0;
	is_single_quote = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' && is_double_quote == 0)
		{
			is_single_quote = !is_single_quote;
			sq = i;
		}
		else if (cmd[i] == '"' && is_single_quote == 0)
		{
			is_double_quote = !is_double_quote;
			sq = i;
		}
	}
	if (is_double_quote || is_single_quote)
		return (sq);
	return (-1);
}

int	has_misplaced_pipes(char *cmd)
{
	int	i;
	int	pipe;
	int	flag;

	flag = 0;
	pipe = 0;
	i = -check_edges(cmd, "|", 0, &flag) - 1;
	if (flag == 1)
		return (check_edges(cmd, "|", 0, &flag));
	while (cmd[++i])
	{
		if (!in_quotes(cmd, i))
		{
			if (cmd[i] == '|' && pipe == 1)
				return (i);
			if (cmd[i] == '|' && pipe == 0)
				pipe = 1;
			// if (ft_strchr("<>", cmd[i]) != NULL && pipe != 0)
			// 	return ((int)cmd[i]);
			if (cmd[i] != '|' && !is_whitespace(cmd[i]))
				pipe = 0;
		}
	}
	return (-1);
}


int	has_invalid_redir(char *cmd)
{
	int	i;
	int	redirfor;
	int	redirback;
	int	flag;

	flag = 0;
	redirfor = 0;
	redirback = 0;
	i = -check_edges(cmd, "<>", 2, &flag) - 1;
	if (flag == 1)
		return (check_edges(cmd, "<>", 2, &flag));
	while (cmd[++i])
	{
		if (!in_quotes(cmd, i))
		{
			if (detect_invalid_redir(cmd, &redirfor, &redirback, i) != -1)
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
	return (-1);
}


int	has_logical_op(char	*cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		if (!in_quotes(cmd, i))
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

int	syntax_error(char	*cmd)
{
	char	*unclosed_quote_err;
	char	*operator_err;
	int		idx;
	int		q_idx;

	unclosed_quote_err = "minishell: syntax error: unclosed quote\n";
	operator_err = "minishell: syntax error near unexpected token ";
	idx = check_err(has_misplaced_pipes(cmd), has_invalid_redir(cmd));
	q_idx = has_unclosed_quotes(cmd);
	if (q_idx == -1 && idx == -1)
		return (1);
	if (q_idx != -1 && (idx > q_idx || idx == -1))
		return (write(2, unclosed_quote_err, ft_strlen(unclosed_quote_err)), 0);
	if (idx > -1 && write(2, operator_err, ft_strlen(operator_err)))
		return (write(2, "`", 1), write(2, &cmd[idx], 1), write(2, "'\n", 2), 0);

	return (1);
}
