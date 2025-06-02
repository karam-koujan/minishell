/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:37:45 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/02 12:15:53 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	g_gl;

void	start_sh(t_sh *sh, char **envp)
{
	sh->cmd_table = NULL;
	sh->token_head = NULL;
	sh->cmd = NULL;
	sh->env = NULL;
	sh->gc = init_gc();
	sh->env = init_env_list(envp);
	if (!sh->env)
		init_empty_env(&sh->env);
	handle_shlvl(&sh->env);
	init_pwd(&sh->env);
	add_to_gc(&sh->gc, NULL, NULL, sh->env);
}

void	clear_sh(t_sh *sh)
{
	g_gl = 0;
	free(sh->cmd);
	clear_parsing(&sh->gc);
	sh->token_head = NULL;
	sh->cmd_table = NULL;
}

int	run(t_sh *sh, t_elem *elem)
{
	sh->token_head = tokenize(sh->cmd, sh->env);
	add_to_gc(&sh->gc, NULL, sh->token_head, NULL);
	if (!sh->token_head)
		return (free(sh->cmd), rl_clear_history(), \
		free_all(&sh->gc), 1);
	sh->cmd_table = parse(sh->token_head, sh->env);
	add_to_gc(&sh->gc, sh->cmd_table, NULL, NULL);
	if (!sh->cmd_table)
		return (free(sh->cmd), rl_clear_history(), \
		free_all(&sh->gc), 1);
	if (handle_herdoc(&sh->cmd_table, &sh->gc))
		return (exit_stat(2, 1, NULL, NULL), clear_sh(sh), 0);
	exec(sh->cmd_table, &sh->env, &sh->gc, &elem);
	clear_sh(sh);
	return (0);
}

int	minishell_loop(t_sh *sh, t_elem *elem)
{
	while (1337)
	{
		sh->cmd = readline("minishell$ ");
		if (sh->cmd == NULL)
			return (printf("exit\n"), exit_stat(0, 0, &(sh->gc), NULL));
		if (!*(sh->cmd))
			continue ;
		add_history(sh->cmd);
		if (!syntax_error(sh->cmd))
		{
			exit_stat(2, 1, NULL, NULL);
			free(sh->cmd);
			continue ;
		}
		if (run(sh, elem))
			return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_sh		sh;
	t_elem		elem;

	if (argc > 1 || !argv)
	{
		perror(argv[0]);
		return (-1);
	}
	g_gl = 0;
	if (!isatty(1) || !isatty(0) || !isatty(2))
		return (1);
	if (!envp || !envp[0])
		elem.aff_path = "oui";
	else
		elem.aff_path = "non";
	elem.flag = 0;
	start_sh(&sh, envp);
	if (handle_signals(&sh))
		return (free_all(&sh.gc), 1);
	return (minishell_loop(&sh, &elem));
}
