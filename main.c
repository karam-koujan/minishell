/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/07 06:50:52 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "./libft/libft.h"

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

int	is_operator(char c)
{
	char	*op;
	int		i;

	i = -1;
	op = "<>|";
	while (op[++i])
	{
		if (op[i] == c)
			return (1);
	}
	return (0);
}

int	has_misplaced_pipes(char *cmd)
{
	int	i;
	int	j;
	int	pipe;
	int	is_double_quote;
	int	is_single_quote;

	i = 0;
	j = ft_strlen(cmd) - 1;
	pipe = 0;
	is_double_quote = 0;
	is_single_quote = 0;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	while (j >= 0 && is_space(cmd[j]))
		j--;
	if (cmd[i] == '|')
		return ((int)cmd[i]);
	if (cmd[j] == '|')
		return ((int)cmd[j]);
	while (cmd[i])
	{
		if (cmd[i] == '\'' && is_double_quote == 0)
			is_single_quote = !is_single_quote;
		else if (cmd[i] == '"' && is_single_quote == 0)
			is_double_quote = !is_double_quote;
		if (!is_single_quote && !is_double_quote)
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
		i++;
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
int main()
{
	char    *cmd;
	while (1337)
	{
	cmd = readline("minishell$ ");
	if (!cmd && !*cmd)
		return (0);
	syntax_error(cmd);
	add_history(cmd);
	free(cmd);
	}
}