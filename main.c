/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/06 02:56:02 by kkoujan          ###   ########.fr       */
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
    free(line);
    }
}