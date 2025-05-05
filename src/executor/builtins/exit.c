/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:19:12 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/05 20:25:44 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include  "../../../includes/minishell.h"


int exit_stat(int value, int action)
{
    static int status = 0;

    if (action == 1) 
        status = value;

    return status; 
}
void exit_status(int status)
 {
    if(WIFSIGNALED(status))
	{
        exit_stat(128 + WTERMSIG(status), 1);
	}
    if (WIFEXITED(status), 1)
         exit_stat(WEXITSTATUS(status), 1);
 }
int is_numeric( char *str)
{
    int i = 0;

    if (!str)
        return 0;
    if (str[0] == '+' || str[0] == '-')
        i++;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}
int ft_atoll(char *str, long long *code)
{
    unsigned long long result = 0;
    int sign = 1;

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
			return 0;
		if (sign == -1 && result > (unsigned long long)LLONG_MAX + 1)
			return 0;
        str++;
    }
    *code = sign * result;
    return 1;
}
void builtin_exit(t_simple_cmd **data, t_gc **gc)
{
    long long code;

    if((*data)->argc == 1)
        return(printf("exit\n"), free_all(gc), exit(0));
    if((*data)->argc == 2)
    {
        if (!is_numeric((*data)->args[1]))
        {
            printf("exit\nexit: %s: numeric argument required\n",
                (*data)->args[1]);
            return (free_all(gc), exit(exit_stat(255, 1)));
        }
        if (!ft_atoll((*data)->args[1], &code))
        {
            printf("exit\nexit: %s: numeric argument required\n",
                (*data)->args[1]);
            return (free_all(gc) ,exit(exit_stat(255, 1)));
        }
    }
    else
        return(printf("exit\nexit: too many arguments\n"), exit(255));
    return (printf("exit\n"), free_all(gc), exit(exit_stat(code % 256, 1)));
}
