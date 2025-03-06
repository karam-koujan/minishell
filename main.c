/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/06 03:30:25 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include  <stdlib.h>
#include "./libft/libft.h"

int has_unclosed_quotes(char    *cmd)
{
    int is_double_quote;
    int is_single_quote;
    int i;

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
    return (!(is_double_quote || is_single_quote));
}


int is_space(char    c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

int has_misplaced_pipes(char    *cmd)
{
    int i;
    int j;
    int pipe;

    i = 0;
    j = ft_strlen(cmd) - 1;
    pipe = 0;
    while (cmd[i] && is_space(cmd[i]))
        i++;
    while (j >= 0 && is_space(cmd[i]))
        j--;
    if (cmd[i] == '|' || cmd[j] == '|')
        return (0);
    while (cmd[i])
    {
        if (cmd[i] == '|' && pipe == 1)
            return (0);
        if (cmd[i] == '|' && pipe == 0)
            pipe = 1;
        if (cmd[i] != '|' && !is_space(cmd[i]))
            pipe = 0;
        i++;
    }
    return (1);
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

    free(line);
    }
}