#include "./includes/minishell.h"

int	g_gl;

void print_cmd_table(t_cmd_table *cmd_table)
{
	int i;
	int j;
	t_redirection *redir;

	if (!cmd_table)
	{
		printf("here Command table is NULL\n");
		return;
	}
	
	printf("Command Table:\n");
	
	if (cmd_table->cmd_count == 0)
	{
		printf(" here No commands in table\n");
		return;
	}
	
	i = 0;
	while (i < cmd_table->cmd_count)
	{
		printf("Command %d:\n", i + 1);
		
		if (!cmd_table->cmds[i])
		{
			printf("  Command is NULL\n");
			i++;
			continue;
		}
		
		// Print arguments
		printf("  Arguments: ");
		if (cmd_table->cmds[i]->argc == 0 || !cmd_table->cmds[i]->args)
		{
			printf("None\n");
		}
		else
		{
			j = 0;
			while (j < cmd_table->cmds[i]->argc)
			{
				if (j > 0)
					printf(" ");
				printf("'%s'", cmd_table->cmds[i]->args[j]);
				j++;
			}
			printf("\n");
		}
		
		// Print redirections
		printf("  Redirections: ");
		if (!cmd_table->cmds[i]->redirs)
		{
			printf("None\n");
		}
		else
		{
			printf("\n");
			
			redir = cmd_table->cmds[i]->redirs;
			while (redir)
			{
				printf("    ");
				
				// Print redirection type
				if (redir->type == REDIR_IN)
					printf("< ");
				else if (redir->type == REDIR_OUT)
					printf("> ");
				else if (redir->type == REDIR_APPEND)
					printf(">> ");
				else if (redir->type == REDIR_HEREDOC)
					printf("<< ");
				printf("is var : %i", redir->is_var);
				// Print file or delimiter
				if (!redir)
				{
					printf("Redirection node is NULL\n");
					 return;
				}

				if (redir->file_or_delimiter)
					printf("%s\n", redir->file_or_delimiter);
				else
					printf("(NULL)\n");  // To debug i                
				redir = redir->next;
			}
		}
		i++;
	}
}

void	handler(int signum)
{
	if (signum == SIGINT && g_gl == 0)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signum == SIGINT && g_gl == 1)
	{
		printf("\n");
		g_gl = 0;
	}
}

int main(int argc, char **argv, char **envp)
{
	char		*cmd;
	t_token		*token_head;
	t_cmd_table	*cmd_table;
	t_env		*env;
	t_gc		*gc;

	(void)argc;
	(void)argv;
	gc = NULL;
	if (signal(SIGINT, handler) == SIG_ERR)
		return (perror("SIGINT ERROR"), 1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("SIGQUIT ERROR"), 1);
	env = init_env_list(envp);
	handle_shlvl(&env);
	init_pwd(&env);
	while (1337)
	{
		cmd = readline("minishell$ ");
		g_gl = 0;
		if (cmd == NULL)
			return (printf("exit\n"), 0);
		if (!*cmd)
			continue ;
		add_history(cmd);
		if (!syntax_error(cmd))
		{
			free(cmd);
			continue ;
		}
		token_head = tokenize(cmd);
		if (!token_head)
			return (free(cmd), rl_clear_history(), 1);
		print_token_list(token_head);
		cmd_table = parse(token_head, env);
		if (!cmd_table)
			return (free(cmd), rl_clear_history(), \
			ft_token_lstclear(&token_head, free), 1);
		print_cmd_table(cmd_table);
		add_to_gc(&gc, (void **)(&token_head), 0);
		add_to_gc(&gc, (void **)(&cmd_table), 1);
		add_to_gc(&gc, (void **)(&env), 2);

		// exec(cmd_table, env);
		free(cmd);
		// ft_token_lstclear(&token_head, free);
		ft_malloc(0, &gc, 1);
		// free_table(cmd_table);
		token_head = NULL;
		cmd_table = NULL;
		gc = NULL;
		env = NULL;
	}
}
