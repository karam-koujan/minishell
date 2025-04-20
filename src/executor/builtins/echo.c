/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:18:43 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/19 18:23:01 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

int is_n_flage(char *str)
{
    int i;

    if(!str || str[0] != '-')
        return (0);
    if(str[0] == '-' && !str[1])
        return (0);
    
    i = 1;
    while(str[i])
    {
        if(str[i] != 'n')
            return(0);
        i++;
    }
    return (1);
}

void builtin_echo(t_simple_cmd **data)
{
    int i;          
    int newline;  

    i = 1;
    newline = 1;
    while((*data)->args[i] && is_n_flage((*data)->args[i]))
    {
        newline = 0; 
        i++;        
    }
    while((*data)->args[i])
    {
        ft_putstr_fd((*data)->args[i], 1);
        if((*data)->args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (newline)
        ft_putstr_fd("\n", 1);
}
