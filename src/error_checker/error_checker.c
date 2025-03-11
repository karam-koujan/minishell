/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:28:03 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/11 00:47:44 by kkoujan          ###   ########.fr       */
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
	if (check_edges(cmd, "|") > 0)
		return (check_edges(cmd, "|"));
	i = -check_edges(cmd, "|") - 1;
	while (cmd[++i])
	{
		if (!in_quotes(cmd, i))
		{
			if (cmd[i] == '|' && pipe == 1)
				return ((int)cmd[i]);
			if (cmd[i] == '|' && pipe == 0)
				pipe = 1;
			if (ft_strchr("<>", cmd[i]) != NULL && pipe != 0)
				return ((int)cmd[i]);
			if (cmd[i] != '|' && !is_space(cmd[i]) && \
				ft_strchr("<>", cmd[i]) == NULL)
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
	if (check_edges(cmd, "<>") > 0)
		return (check_edges(cmd, "<>"));
	i = -check_edges(cmd, "<>") - 1;
	while (cmd[++i])
	{
		if (!in_quotes(cmd, i))
		{
			if (detect_invalid_redir(cmd, &redirfor, &redirback, i) != 0)
				return (detect_invalid_redir(cmd, &redirfor, &redirback, i));
			increment_redir(cmd[i], &redirfor, &redirback);
			if ((cmd[i] != '<' && cmd[i] != '>') && !is_space(cmd[i]) && \
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
int	syntax_error(char	*cmd)
{
	char	*unclosed_quote_err;
	char	*log_op_err;
	char	*operator_err;
	int		operator;

	unclosed_quote_err = "minishell: syntax error: unclosed quote\n";
	operator_err = "minishell: syntax error near unexpected token ";
	log_op_err = "minishell: syntax error : unsupported operator ";
	operator = has_invalid_redir(cmd);
	if (has_unclosed_quotes(cmd))
		return (write(2, unclosed_quote_err, ft_strlen(unclosed_quote_err)), 1);
	if (operator && write(2, operator_err, ft_strlen(operator_err)))
		return (write(2, &operator, 1), write(2,"\n",1), 1);
	operator = has_misplaced_pipes(cmd);
	if (operator && write(2, operator_err, ft_strlen(operator_err)))
		return (write(2, &operator, 1), write(2,"\n",1), 1);
	operator = has_logical_op(cmd);
	if (operator && write(2, log_op_err, ft_strlen(log_op_err)))
		return (write(2, &operator, 1), write(2, &operator, 1), write(2,"\n",1), 1);
	return (0);
}