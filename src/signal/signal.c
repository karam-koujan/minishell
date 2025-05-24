/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:14:56 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/24 16:18:11 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handler(int signum, siginfo_t *info, void	*context)
{
	(void)info;
	(void)context;
	if (signum == SIGINT && g_gl != 2)
		exit_stat(130, 1, NULL, NULL);
	if (signum == SIGINT && g_gl == 2)
		printf("\n");
	if (signum == SIGINT && g_gl == 0)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signum == SIGINT && g_gl == 1)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
	}

	if (signum == SIGINT && g_gl == 4)
	{
		g_gl = 3;
	}
}

int	handle_signals(t_sh *sh)
{
	sh->sa.sa_sigaction = handler;
	sh->sa.sa_flags = SA_RESTART;
	sigemptyset(&sh->sa.sa_mask);
	if (sigaction(SIGINT, &sh->sa, NULL) == -1)
		return (perror("SIGINT ERROR"), 1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("SIGQUIT ERROR"), 1);
	return (0);
}