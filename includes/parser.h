/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 03:08:32 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/20 16:56:09 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "./tokenizer.h"
# include "./env.h"
# include <unistd.h>

typedef enum e_redir_type {
    REDIR_IN, 
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redir_type;

typedef struct s_redirection {
    t_redir_type type;
    char *file_or_delimiter;
    int is_var;
    struct s_redirection *next;
} t_redirection;

typedef struct s_simple_cmd {
    char **args;
    int argc;           
    t_redirection *redirs;
} t_simple_cmd;

typedef struct s_cmd_table {
    t_simple_cmd **cmds; 
    int cmd_count;
} t_cmd_table;


t_cmd_table	*parse(t_token *tokenlst, t_env *env);
t_token *parse_redir(t_simple_cmd **cmd,t_token *token);
t_token *parse_word(t_simple_cmd **cmd,t_token *token, t_env *env);
void	add_cmd_to_table(t_cmd_table *cmd_table, t_simple_cmd *cmd);
void	add_arg_to_cmd(t_simple_cmd *cmd, char *arg);
void	add_redir_to_cmd(t_simple_cmd *cmd, t_redirection *redir);
t_cmd_table *create_command_table();
t_simple_cmd *create_simple_cmd();
t_redirection *create_redirection(t_redir_type type, char *file_or_delimiter, int is_var);
void	free_table(t_cmd_table *cmd_table);
void	free_simple_cmd(t_simple_cmd *cmd);
void	free_redirections(t_redirection *redirs);
t_token	*parse_token(t_cmd_table **cmd_table, \
	t_simple_cmd **simple_cmd, t_token *token, t_env *env);
#endif