/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:31:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/18 09:06:30 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

int	is_imbigious(char	*var)
{
	char	**arr;

	arr = NULL;
	if (*var == 0)
		return (1);
	arr = ft_split(var, ' ');
	if (!arr)
		return (free(var), -1);
	if (arr[1] == NULL)
		return (free_arr(arr), free(var), 0);
	else
		return (free_arr(arr), free(var), 2);
	return (free_arr(arr), free(var), 0);
}

int	detect_ambigious_redir(t_token *token, t_env *env)
{
	int		is_imbig;
	int		is_quote;

	is_imbig = 0;
	is_quote = 0;
	while (token && (token->type == VAR_T || \
		token->type == WORD_T || token->type == QT_T))
	{
		if (token->type == WORD_T)
			is_quote = 1;
		if (token->type == VAR_T && !token->v_in_qt)
		{
			if (is_imbigious(ft_getenv_val(env, token->val)) == 0)
				is_quote = 1;
			if (is_imbigious(ft_getenv_val(env, token->val)) == 2)
				return (1);
			else if (is_imbigious(ft_getenv_val(env, token->val)) == -1)
				return (-1);
			else
				is_imbig = (is_imbigious(ft_getenv_val(env, token->val)) \
				|| is_imbig);
		}
		token = token->next;
	}
	return (is_imbig && !is_quote);
}


t_redirection	*redir_file(t_token **token, t_env *env, t_redir_type type)
{
	t_redirection	*redir;
	char			*val;
	int				is_ambigious;
	t_token			*next;

	*token = (*token)->next;
	while (*token && ((*token)->type == SP_T || (*token)->type == QT_T))
		*token = (*token)->next;
	is_ambigious = detect_ambigious_redir(*token, env);
	if (is_ambigious == -1)
		return (NULL);
	val = join_expnd(*token, env);
	redir = create_redirection(type, val, is_ambigious);
	free(val);
	while (*token)
	{
		next = (*token)->next;
		if (next && (next->type != WORD_T || next->type != VAR_T \
		|| next->type != QT_T))
			break ;
		*token = (*token)->next;
	}
	return (redir);
}

t_token	*parse_redir(t_simple_cmd **cmd, t_token *token, t_env *env)
{
	t_redirection	*redir;
	t_redir_type	redir_type;
	char			*val;

	if (token->type == REDIR_B_T)
		redir_type = REDIR_IN;
	else if (token->type == REDIR_F_T)
		redir_type = REDIR_OUT;
	else if (token->type == APPEND_T)
		redir_type = REDIR_APPEND;
	else
		redir_type = REDIR_HEREDOC;
	redir = redir_file(&token, env, redir_type);
	if (redir == NULL || cmd == NULL)
		return (NULL);
	if (*cmd == NULL)
		*cmd = create_simple_cmd();
	add_redir_to_cmd(*cmd, redir);
	return (token);
}
