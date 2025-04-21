/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:36:09 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/21 10:44:58 by kkoujan          ###   ########.fr       */
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
# include "./executor.h"
# include "./env.h"

extern int  g_gl;

#endif
