/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:36:09 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/24 17:39:57 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include "../lib/libft/libft.h"
# include "./error_checker.h"
# include "./tokenizer.h"
# include "./parser.h"
# include "./env.h"
# include "./executor.h"
# include "./helper.h"

extern int					g_gl;
typedef struct sigaction	t_sigaction;
typedef struct s_sh
{
	char		*cmd;
	t_token		*token_head;
	t_cmd_table	*cmd_table;
	t_env		*env;
	t_sigaction	sa;
	t_gc		*gc;
}				t_sh;
void	free_in_exit(t_token *token_head, t_cmd_table *cmd_table, \
	t_env *env);
void	handler(int signum, siginfo_t *info, void	*context);
int		handle_signals(t_sh *sh);
#endif
