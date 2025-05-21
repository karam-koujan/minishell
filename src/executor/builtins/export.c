/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:20:02 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/20 16:12:55 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

void builtin_export(t_simple_cmd **data, t_env **env,  t_gc **gc)
{
	int i; 
    
	i = 1;
	if(!(*data)->args[1])
		return(print_export((*env)));
	while((*data)->args[i])
	{
		if(!valid_export((*data)->args[i], gc))
		{
			i++;
			continue ;
		}
		else
			var_set((*data)->args[i], env, gc);
		i++;
	}
}
