/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 03:09:20 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/30 18:40:15 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

t_redirection *create_redirection(t_redir_type type, char *file_or_delimiter)
{
    t_redirection   *result;
    
    result = malloc(sizeof(t_redirection));
    if (!result)
        return (NULL);
    result->type = type;
    result->file_or_delimiter = ft_strdup(file_or_delimiter);
    if (!result->file_or_delimiter)
        return (NULL);
    result->next = NULL;
    return (result);   
}

t_simple_cmd *create_simple_cmd()
{
    t_simple_cmd    *result;
    result = malloc(sizeof(t_simple_cmd));
    if (!result)
        return (NULL);
    result->args = NULL;
    result->argc = 0;
    result->redirs = NULL;
    return (result);
}
t_cmd_table *create_command_table()
{
    t_cmd_table    *result;
    result = malloc(sizeof(t_cmd_table));
    if (!result)
        return (NULL);
    result->cmds = NULL;
    result->cmd_count = 0;
    return (result);
}

/*
- **add_redirection_to_command**: Adds a redirection to a simple command.
- **add_argument_to_command**: Adds an argument to a simple command's argument list.
- **add_command_to_table**: Adds a simple command to a command table.
*/

void add_redir_to_cmd(t_simple_cmd *cmd, t_redirection *redir)
{
    t_redirection *redirs;
    if (!cmd || !redir)
        return ;
    if (!cmd->redirs)
        cmd->redirs = redir;
    else
    {
        redirs = cmd->redirs;
        while (redirs != NULL && redirs->next != NULL)
            redirs = redirs->next;
        redirs->next = redir;
    }
}
void add_arg_to_cmd(t_simple_cmd *cmd, char *arg)
{
    int     arg_c;
    char    **new_arg;
    int     i;

    i = -1;
    arg_c = cmd->argc;
    new_arg = malloc((arg_c + 2) * sizeof(char *));
    if (!new_arg)
        return ;
    while (++i < arg_c)
        new_arg[i] = cmd->args[i];
    new_arg[i] = arg;
    new_arg[++i] = NULL;   
    cmd->args = new_arg; 
    cmd->argc = arg_c + 1;
}
void add_cmd_to_table(t_cmd_table *cmd_table, t_simple_cmd *cmd)
{
    int     cmd_c;
    t_simple_cmd    **new_cmds;
    int     i;

    i = -1;
    cmd_c = cmd_table->cmd_count;
    new_cmds = malloc((cmd_c + 2) * sizeof(t_simple_cmd *));
    if (!new_cmds)
        return ;
    while (++i < cmd_c)
        new_cmds[i] = cmd_table->cmds[i];
    new_cmds[i] = cmd;
    new_cmds[++i] = NULL;
    cmd_table->cmds = new_cmds;  
    cmd_table->cmd_count = cmd_c + 1;
}

t_token *parse_word(t_simple_cmd **cmd,t_token *token)
{    
    if(*cmd == NULL)
        *cmd = create_simple_cmd();
    if (*cmd == NULL)
        return (NULL);
    add_arg_to_cmd(*cmd, token->val);
    return (token);
}

t_token *parse_redir(t_simple_cmd **cmd,t_token *token)
{
    t_redirection   *redir;
    t_redir_type    redir_type;
    if (token->type == REDIR_B_T)
        redir_type = REDIR_IN;
    else if (token->type == REDIR_F_T)
        redir_type = REDIR_OUT;
    else if (token->type == APPEND_T)
        redir_type = REDIR_APPEND;
    else
        redir_type = REDIR_HEREDOC;
    while (token && token->type != WORD_T)
        token = token->next;
    redir = create_redirection(redir_type, token->val);
    if (redir == NULL || cmd == NULL)
        return (NULL);
    add_redir_to_cmd(*cmd, redir);
    return (token);
}

t_cmd_table *parse(t_token *tokenlst)
{
    t_token *token;
    t_cmd_table *cmd_table;
    t_simple_cmd *simple_cmd;

    token = tokenlst;
    cmd_table = create_command_table();
    simple_cmd = NULL;
    while (token)
    {
        if (token->type == WORD_T)
        {
            token = parse_word(&simple_cmd, token);
            if (!token)
                return (NULL);
        }
        else if (token->type == APPEND_T || token->type == HERDOC_T \
            || token->type == REDIR_B_T || token->type == REDIR_F_T)
        {   
            token = parse_redir(&simple_cmd, token);
            if (!token)
                return (NULL);
        }
        else if (token->type == PIPE_T)
        {
            add_cmd_to_table(cmd_table, simple_cmd);
            simple_cmd = NULL;   
        }
        token = token->next;
    }
    if (simple_cmd != NULL)
        add_cmd_to_table(cmd_table, simple_cmd); 
    return (cmd_table);
}
