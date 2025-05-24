#include "./includes/minishell.h"

int	g_gl;

// void print_cmd_table(t_cmd_table *cmd_table)
// {
// 	int i;
// 	int j;
// 	t_redirection *redir;

// 	if (!cmd_table)
// 	{
// 		printf("here Command table is NULL\n");
// 		return;
// 	}
	
// 	printf("Command Table:\n");
	
// 	if (cmd_table->cmd_count == 0)
// 	{
// 		printf(" here No commands in table\n");
// 		return;
// 	}
	
// 	i = 0;
// 	while (i < cmd_table->cmd_count)
// 	{
// 		printf("Command %d:\n", i + 1);
		
// 		if (!cmd_table->cmds[i])
// 		{
// 			printf("  Command is NULL\n");
// 			i++;
// 			continue;
// 		}
		
// 		// Print arguments
// 		printf("  Arguments: ");
// 		if (cmd_table->cmds[i]->argc == 0 || !cmd_table->cmds[i]->args)
// 		{
// 			printf("None\n");
// 		}
// 		else
// 		{
// 			j = 0;
// 			while (j < cmd_table->cmds[i]->argc)
// 			{
// 				if (j > 0)
// 					printf(" ");
// 				printf("'%s'", cmd_table->cmds[i]->args[j]);
// 				j++;
// 			}
// 			printf("\n");
// 		}
		
// 		// Print redirections
// 		printf("  Redirections: ");
// 		if (!cmd_table->cmds[i]->redirs)
// 		{
// 			printf("None\n");
// 		}
// 		else
// 		{
// 			printf("\n");
			
// 			redir = cmd_table->cmds[i]->redirs;
// 			while (redir)
// 			{
// 				printf("    ");
				
// 				// Print redirection type
// 				if (redir->type == REDIR_IN)
// 					printf("< ");
// 				else if (redir->type == REDIR_OUT)
// 					printf("> ");
// 				else if (redir->type == REDIR_APPEND)
// 					printf(">> ");
// 				else if (redir->type == REDIR_HEREDOC)
// 					printf("<< ");
// 				printf("is var : %i", redir->is_ambigous);
// 				// Print file or delimiter
// 				if (!redir)
// 				{
// 					printf("Redirection node is NULL\n");
// 					 return;
// 				}

// 				if (redir->file_or_delimiter)
// 				{
// 					printf("%s\n", redir->file_or_delimiter);
// 					printf("%i\n", redir->herdoc_fd);
// 				}
// 				else
// 					printf("(NULL)\n");  // To debug i                
// 				redir = redir->next;
// 			}
// 		}
// 		i++;
// 	}
// }

void	start_sh(t_sh *sh, char **envp)
{
	sh->cmd_table = NULL;
	sh->token_head = NULL;
	sh->cmd = NULL;
	sh->gc = init_gc();
	sh->env = init_env_list(envp, &sh->gc);
	if (!sh->env)
		init_empty_env(&sh->env, &sh->gc);
	handle_shlvl(&sh->env, &sh->gc);
	init_pwd(&sh->env, &sh->gc);
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

int	run(t_sh *sh)
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
	handle_herdoc(&sh->cmd_table, NULL, &sh->gc);
	exec(sh->cmd_table, &sh->env, &sh->gc);
	clear_sh(sh);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_sh		sh;

	(void)argc;
	(void)argv;
	g_gl = 0;
	start_sh(&sh, envp);
	if (handle_signals(&sh))
		return (1);
	while (1337)
	{
		sh.cmd = readline("minishell$ ");
		if (sh.cmd == NULL)
			return (printf("exit\n"), exit_stat(0, 0, &sh.gc, NULL));
		if (!*sh.cmd)
			continue ;
		add_history(sh.cmd);
		if (!syntax_error(sh.cmd))
		{
			exit_stat(2, 1, NULL, NULL);
			free(sh.cmd);
			continue ;
		}
		if (run(&sh))
			return (1);
	}
}
