/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/13 02:28:07 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"
#include <stdio.h>

int main()
{
	char    *cmd;

	while (1337)
	{
	cmd = readline("minishell$ ");
	if (!cmd || !*cmd)
		return (0);
	syntax_error(cmd);
	printf("token :\n");
	print_token_list(tokenize(cmd));
	add_history(cmd);
	free(cmd);
	}
}