/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:13:00 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/16 15:12:26 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/error_checker.h"

void	increment_redir(char c, int *redirfor, int *redirback)
{
	if (c == '>' && (*redirfor < 2 || *redirback == 0))
		(*redirfor)++;
	if (c == '<' && (*redirback < 2 || *redirfor == 0))
		(*redirback)++;
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


int	check_edges(char	*cmd, char	*charset, int edge)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(cmd) - 1;
	while (cmd[i] && is_whitespace(cmd[i]))
		i++;
	while (j >= 0 && is_whitespace(cmd[j]))
		j--;

	if (ft_strchr(charset, cmd[i]) != NULL && (edge == 1 || edge == 0))
		return (i);
	if (ft_strchr(charset, cmd[j]) != NULL && (edge == 2 || edge == 0))
		return (j);
	return (-i);
}

int	detect_invalid_redir(char *cmd, int *redirfor, int *redirback, int i)
{
	int	rf;
	int	rb;

	rf = *redirfor;
	rb = *redirback;
	if (cmd[i] == '>' && (rf >= 2 || rb != 0))
		return (i);
	if (i > 0 && cmd[i] == '>' && is_whitespace(cmd[i - 1]) && rf > 0)
		return (i);
	if (i > 0 && cmd[i] == '<' && is_whitespace(cmd[i - 1]) && rb > 0)
		return (i);
	if (ft_strchr("|", cmd[i]) != NULL && (rf != 0 \
	|| rb != 0))
		return (i);
	if (cmd[i] == '<' && (rb >= 2 || rf != 0))
		return (i);
	return (-1);
}
