/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:28:03 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/10 22:18:21 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"

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


int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	in_quotes(char *cmd, int pos)
{
	int	i;
	int	single_q;
	int	double_q;

	i = 0;
	single_q = 0;
	double_q = 0;
	while (i < pos && cmd[i])
	{
		if (cmd[i] == '\'' && !double_q)
			single_q = !single_q;
		else if (cmd[i] == '"' && !single_q)
			double_q = !double_q;
		i++;
	}
	return (single_q || double_q);
}


int	check_edges(char	*cmd, char	*charset)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(cmd) - 1;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	while (j >= 0 && is_space(cmd[j]))
		j--;

	if (ft_strchr(charset, cmd[i]) != NULL)
		return ((int)cmd[i]);
	if (ft_strchr(charset, cmd[j]) != NULL)
		return ((int)cmd[j]);
	return (-i);
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
	int	j;
	int	redirfor;
	int	redirback;
	int	is_double_quote;
	int	is_single_quote;

	i = 0;
	j = ft_strlen(cmd) - 1;
	redirfor = 0;
	redirback = 0;
	is_double_quote = 0;
	is_single_quote = 0;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	while (j >= 0 && is_space(cmd[j]))
		j--;
	if (cmd[i] == '<' || cmd[i] == '>')
		return ((int)cmd[i]);
	if ( cmd[j] == '<' || cmd[j] == '>')
		return ((int)cmd[j]);
	while (cmd[i])
	{
		if (cmd[i] == '\'' && is_double_quote == 0)
			is_single_quote = !is_single_quote;
		else if (cmd[i] == '"' && is_single_quote == 0)
			is_double_quote = !is_double_quote;
		if (!is_single_quote && !is_double_quote)
		{
			if (cmd[i] == '>' && (redirfor >= 2 || redirback != 0))
				return ((int)cmd[i]);
			if (i > 0 && cmd[i] == '>' && is_space(cmd[i-1])  && redirfor > 0)
				return ((int)cmd[i]);
			if (i > 0 && cmd[i] == '<' && is_space(cmd[i-1]) && redirback > 0)
				return ((int)cmd[i]);
			if (ft_strchr("|", cmd[i]) != NULL && (redirfor != 0 \
			|| redirback != 0))
				return ((int)cmd[i]);
			if (cmd[i] == '<' && (redirback >= 2 || redirfor != 0))
				return ((int)cmd[i]);
			if (cmd[i] == '>' && (redirfor < 2 || redirback == 0))
				redirfor++;
			if (cmd[i] == '<' && (redirback < 2 || redirfor == 0))
				redirback++;
			if ((cmd[i] != '<' && cmd[i] != '>') && !is_space(cmd[i]) && \
			ft_strchr("|", cmd[i]) == NULL)
			{
				redirback = 0;
				redirfor = 0;
			}
		}
		i++;
	}
	return (0);
}

int	has_logical_op(char	*cmd)
{
	size_t	i;
	int		is_single_quote;
	int		is_double_quote;

	i = 0;
	is_single_quote = 0;
	is_double_quote = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && is_double_quote != 0)
			is_single_quote = !is_single_quote;
		if (cmd[i] == '"' && is_single_quote != 0)
			is_double_quote = !is_double_quote;
		if (!is_double_quote && !is_single_quote)
		{
			if (i + 1 < ft_strlen(cmd) && cmd[i] == '|' && cmd[i + 1] == '|')
				return ((int)cmd[i]);
			if (i + 1 < ft_strlen(cmd) && cmd[i] == '&' && cmd[i + 1] == '&')
				return ((int)cmd[i]);
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