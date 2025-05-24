/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_extra.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:42:47 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/24 13:43:19 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_word_val(t_token *token, t_env *env)
{
	char	*value;

	value = NULL;
	if (token->type == VAR_T)
	{
		if (token->val && ft_strcmp(token->val, "?") == 0)
			value = ft_itoa(exit_stat(0, 0, NULL, NULL));
		else if (token->v_in_qt)
			value = ft_getenv_val(env, token->val);
		else if (!token->v_in_qt)
			value = expnd_out_qt(env, token->val);
		if (!value)
			return (NULL);
	}
	else
	{
		value = ft_strdup(token->val);
		if (!value)
			return (NULL);
	}
	return (value);
}
