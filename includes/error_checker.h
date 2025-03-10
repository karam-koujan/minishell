/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checker.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:29:10 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/10 21:33:55 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_CHECKER_H
# define ERROR_CHECKER_H

int	has_unclosed_quotes(char *cmd);
int	is_space(char c);
int	has_misplaced_pipes(char *cmd);
int	has_invalid_redir(char *cmd);
int	has_logical_op(char	*cmd);
int	syntax_error(char	*cmd);

#endif