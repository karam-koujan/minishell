/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:20:02 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 15:25:00 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

void	builtin_export_child(t_simple_cmd **data, t_env **env, t_gc **gc,
		t_elem **elem)
{
	int	i;

	i = 1;
	if (!(*data)->args[1])
		return (print_export((*env)));
	while ((*data)->args[i])
	{
		if (!valid_export((*data)->args[i], gc))
		{
			write(2, "minihell: export: ", 19);
			write(2, "\'", 1);
			write(2, (*data)->args[i], ft_strlen((*data)->args[i]));
			write(2, "\'", 1);
			write(2, ": not a valid identifier", 25);
			write(2, "\n", 1);
			exit(exit_stat(1, 1, gc, (*elem)->env));
		}
		i++;
	}
}

void	builtin_export(t_simple_cmd **data, t_env **env, t_gc **gc)
{
	int	i;

	i = 1;
	if (!(*data)->args[1])
		return (print_export((*env)));
	while ((*data)->args[i])
	{
		if (!valid_export((*data)->args[i], gc))
		{
			printf("export: '%s': not a valid identifier\n", (*data)->args[i]);
			exit_stat(1, 1, NULL, NULL);
			i++;
			continue ;
		}
		else
			var_set((*data)->args[i], env, gc);
		i++;
	}
}
