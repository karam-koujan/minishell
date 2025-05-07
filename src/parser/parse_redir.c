/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:31:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/07 17:36:26 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

int	is_imbigious(char	*var, t_token *token)
{
	char	**arr;

	arr = NULL;
	if (var == NULL)
		return (-1);
	if (*var == 0 && token->next && token->next->type == WORD_T)
		return (0);
	if (*var == 0)
		return (1);
	arr = ft_split(var, ' ');
	if (!arr)
		return (free(var), -1);
	if (arr[1] == NULL)
		return (free_arr(arr), free(var), 0);
	else
		return (free_arr(arr), free(var), 1);
	return (free_arr(arr), free(var), 0);
}

int	detect_ambigious_redir(t_token *token, t_env *env)
{
	while (token)
	{
		if (token->type == VAR_T && !token->v_in_qt)
			return (is_imbigious(ft_getenv_val(env, token->val), token));
		token = token->next;
	}
	return (0);
}

t_redirection	*redir_file(t_token **token, t_env *env, t_redir_type type)
{
	t_redirection	*redir;
	char			*val;
	int				in_quote;
	int				is_ambigious;

	while (*token && ((*token)->type != WORD_T && (*token)->type != VAR_T))
		*token = (*token)->next;
	is_ambigious = detect_ambigious_redir(*token, env);
	if (is_ambigious == -1)
		return (NULL);
	val = join_expnd(*token, env);
	printf("val : %s", val);
	redir = create_redirection(type, val, is_ambigious);
	free(val);
	while (*token && ((*token)->type == WORD_T || (*token)->type == VAR_T || (*token)->type == QT_T))
		*token = (*token)->next;
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
