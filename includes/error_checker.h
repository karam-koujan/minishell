/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:29:10 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/17 16:21:31 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_CHECKER_H
# define ERROR_CHECKER_H

# include <unistd.h>
# include "../lib/libft/libft.h"
# include "../includes/helper.h"

int		has_unclosed_quotes(char *cmd);
int		has_misplaced_pipes(char *cmd);
int		has_invalid_redir(char *cmd);
int		has_logical_op(char	*cmd);
int		syntax_error(char	*cmd);
void    increment_redir(char c, int *redirfor, int *redirback);
int		in_quotes(char *cmd, int pos);
int		check_edges(char	*cmd, char	*charset, int edge, int *flag);
int		detect_invalid_redir(char *cmd, int *redirfor, int *redirback, int i);
int     check_err(int p_idx, int r_idx);

#endif
