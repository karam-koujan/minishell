/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:19:12 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/27 13:31:30 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

static void print_exit_error(char *msg, char *arg, char *type)
{
	if(type == "PR")
	{
		write(2, "exit\nminishell: exit: ", 23);
		if(arg)
			write(2, arg, ft_strlen(arg));
		write(2, msg, ft_strlen(msg));
	}
	else
	{
		write(2, "minishell: exit: ", 18);
		if(arg)
			write(2, arg, ft_strlen(arg));
		write(2, msg, ft_strlen(msg));
	}
}

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
	if (!is_numeric((*data)->args[1]) || !ft_atoll((*data)->args[1], &code))
	{
		print_exit_error(": numeric argument required\n", (*data)->args[1], "CH");
		return (exit(exit_stat(2, 1, gc, (*elem)->env)));
	}
	else if ((*data)->argc > 2)
	{
		print_exit_error(": too many arguments\n", NULL, "CH");
		exit_stat(1, 1, NULL, NULL);
		return ;
	}
	return (exit(exit_stat(code % 256, 1, gc, (*elem)->env)));
}

void	builtin_exit(t_simple_cmd **data, t_gc **gc, t_elem **elem)
{
	long long	code;

	if ((*data)->argc == 1)
		return (printf("exit\n"), exit(exit_stat(0, 0, gc, (*elem)->env)));
	if (!is_numeric((*data)->args[1]) || !ft_atoll((*data)->args[1], &code))
	{
		print_exit_error(": numeric argument required\n", (*data)->args[1], "PR");
		return (exit(exit_stat(2, 1, gc, (*elem)->env)));
	}
	else if ((*data)->argc > 2)
	{
		print_exit_error(": too many arguments\n", NULL, "PR");
		exit_stat(1, 1, NULL, NULL);
		return ;
	}
	return (write(2, "exit\n", 6), exit(exit_stat(code % 256, 1, gc, (*elem)->env)));
}
