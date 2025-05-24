/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:43:34 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/24 13:42:29 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redirections(t_redirection *redirs)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		free(current->file_or_delimiter);
		free(current);
		current = next;
	}
}

void	free_simple_cmd(t_simple_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->argc)
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	if (cmd->redirs)
		free_redirections(cmd->redirs);
	free(cmd);
}

void	free_table(t_cmd_table *cmd_table)
{
	int	i;

	if (!cmd_table)
		return ;
	i = 0;
	while (i < cmd_table->cmd_count)
	{
		free_simple_cmd(cmd_table->cmds[i]);
		i++;
	}
	free(cmd_table->cmds);
	free(cmd_table);
}

char	*ft_getenv_val(t_env *env, char *key)
{
	if (!env || !key)
		return (NULL);
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(exit_stat(0, 0, NULL, NULL)));
	while (env)
	{
		if (ft_strlen(env->key) - 1 == ft_strlen(key) && \
		ft_strncmp(env->key, key, ft_strlen(env->key) - 1) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*expnd_out_qt(t_env *env, char *key)
{
	char	*value;
	char	*tmp;

	tmp = ft_getenv_val(env, key);
	if (!tmp)
		return (NULL);
	value = ft_strtrim(tmp, " ");
	if (!value)
		return (free(tmp), NULL);
	return (free(tmp), value);
}

