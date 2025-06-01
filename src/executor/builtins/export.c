/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:20:02 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/31 15:57:24 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static void	print_export_error(char *arg)
{
	write(2, "minihell: export: ", 19);
	write(2, "\'", 1);
	write(2, arg, ft_strlen(arg));
	write(2, "\'", 1);
	write(2, ": not a valid identifier", 25);
	write(2, "\n", 1);
}

void	builtin_export_child(t_simple_cmd **data, t_env **env, t_gc **gc,
		t_elem **elem)
{
	int	i;

	i = 1;
	if (!(*data)->args[1])
		return (print_export((*env), elem));
	while ((*data)->args[i])
	{
		if (!valid_export((*data)->args[i]))
		{
			print_export_error((*data)->args[i]);
			exit(exit_stat(1, 1, gc, (*elem)->env));
		}
		i++;
	}
}

void	builtin_export(t_simple_cmd **data, t_env **env, t_gc **gc,
		t_elem **elem)
{
	int	i;

	i = 1;
	if (!(*data)->args[1])
		return (print_export((*env), elem));
	while ((*data)->args[i])
	{
		if (!valid_export((*data)->args[i]))
		{
			print_export_error((*data)->args[i]);
			exit_stat(1, 1, NULL, NULL);
			i++;
			continue ;
		}
		else
		{
			if (ft_strcmp((*data)->args[1], "PATH") == 0)
				(*elem)->flag = 1;
			var_set((*data)->args[i], env, elem);
			(*gc)->env = *env;
		}
		i++;
	}
}
