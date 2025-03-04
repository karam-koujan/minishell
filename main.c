/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/04 15:05:28 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include  <stdlib.h>

int main()
{
    char    *line;
    while (1337)
    {
    line = readline("minishell$ ");
    if (!line)
        return (0);
    printf("line %s\n", line);
    free(line);
    }
}