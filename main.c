/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:08:27 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/21 22:45:30 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"
#include <stdio.h>

int main()
{
	char    *cmd;
	t_token	*token_head;

	while (1337)
	{
	cmd = readline("minishell$ ");
	if (!cmd || !*cmd)
		return (0);
	syntax_error(cmd);
	printf("token :\n");
	token_head = tokenize(cmd);
	print_token_list(token_head);
	add_history(cmd);
	free(cmd);
	ft_token_lstclear(&token_head, free);
	}
}
