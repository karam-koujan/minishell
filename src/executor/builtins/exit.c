/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:19:12 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 15:15:36 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static int	is_numeric( char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoll(char *str, long long *code)
{
	unsigned long long	result;
	int					sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (sign == 1 && result > (unsigned long long)LLONG_MAX)
			return (0);
		if (sign == -1 && result > (unsigned long long)LLONG_MAX + 1)
			return (0);
		str++;
	}
	*code = sign * result;
	return (1);
}

void	builtin_exit_child(t_simple_cmd **data, t_gc **gc, t_elem **elem)
{
	long long	code;

	if ((*data)->argc == 1)
		return (exit(exit_stat(0, 0, gc, (*elem)->env)));
	if ((*data)->argc == 2)
	{
		if (!is_numeric((*data)->args[1]))
		{
			write(2, "minishell: exit: ", 18);
			write(2, (*data)->args[1], ft_strlen((*data)->args[1]));
			write (2, ": numeric argument required\n", 29);
			return (exit(exit_stat(255, 1, gc, (*elem)->env)));
		}
		if (!ft_atoll((*data)->args[1], &code))
		{
			write(2, "minishell: exit: ", 18);
			write(2, (*data)->args[1], ft_strlen((*data)->args[1]));
			write (2, ": numeric argument required\n", 29);
			return (exit(exit_stat(255, 1, gc, (*elem)->env)));
		}
	}
	else
		return (write(2, "minishell: exit: too many arguments\n", 37),
			exit(exit_stat(255, 1, gc, (*elem)->env)));
	return (exit(exit_stat(code % 256, 1, gc, (*elem)->env)));
}

void	builtin_exit(t_simple_cmd **data, t_gc **gc, t_elem **elem)
{
	long long	code;

	if ((*data)->argc == 1)
		return (printf("exit\n"), exit(exit_stat(0, 0, gc, (*elem)->env)));
	if ((*data)->argc == 2)
	{
		if (!is_numeric((*data)->args[1]))
		{
			printf("exit\nminishell: exit: %s: numeric argument required\n",
				(*data)->args[1]);
			return (exit(exit_stat(255, 1, gc, (*elem)->env)));
		}
		if (!ft_atoll((*data)->args[1], &code))
		{
			printf("exit\nminishell: exit: %s: numeric argument required\n",
				(*data)->args[1]);
			return (exit(exit_stat(255, 1, gc, (*elem)->env)));
		}
	}
	else
		return (printf("exit\nexit: too many arguments\n"),
			exit(exit_stat(255, 1, gc, (*elem)->env)));
	return (printf("exit\n"), exit(exit_stat(code % 256, 1, gc, (*elem)->env)));
}
