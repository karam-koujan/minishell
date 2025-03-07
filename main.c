/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/06 03:52:17 by kkoujan          ###   ########.fr       */
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

int	is_unvalid_arg(char c)
{
	return (is_space(c) || is_operator(c));
}

int	has_misplaced_pipes(char *cmd)
{
	int	i;
	int	j;
	int	pipe;

	i = 0;
	j = ft_strlen(cmd) - 1;
	pipe = 0;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	while (j >= 0 && is_space(cmd[j]))
		j--;
	if (cmd[i] == '|' || cmd[j] == '|')
		return (1);
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			break ;
		if (cmd[i] == '|' && pipe == 1)
			return (1);
		if (cmd[i] == '|' && pipe == 0)
			pipe = 1;
		if (ft_strchr("<>", cmd[i]) != NULL && pipe != 0)
			return (1);
		if (cmd[i] != '|' && !is_space(cmd[i]) && \
		ft_strchr("<>", cmd[i]) == NULL)
			pipe = 0;
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

	i = 0;
	j = ft_strlen(cmd) - 1;
	redirfor = 0;
	redirback = 0;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	while (j >= 0 && is_space(cmd[j]))
		j--;
	if (cmd[i] == '<' || cmd[i] == '>' || cmd[j] == '<' || cmd[j] == '>')
		return (1);
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			break ;
		if (cmd[i] == '>' && (redirfor > 2 || redirback != 0))
			return (1);
		if (i > 0 && cmd[i] == '>' && is_space(cmd[i-1])  && redirfor > 0)
			return (1);
		if (i > 0 && cmd[i] == '<' && is_space(cmd[i-1]) && redirback > 0)
			return (1);
		if (ft_strchr("|", cmd[i]) != NULL && (redirfor != 0 || redirback != 0))
			return (1);
		if (cmd[i] == '<' && (redirback > 2 || redirfor != 0))
			return (1);
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
		i++;
	}
	return (0);
}

int main()
{
	char    *line;
	while (1337)
	{
	line = readline("minishell$ ");
	if (!line)
		return (0);
	printf("line %s\n", line);
	printf("line %i\n", has_unclosed_quotes(line));
	printf("line %i\n", has_misplaced_pipes(line));
	printf("redir %i\n", has_invalid_redir(line));

	free(line);
	}
}