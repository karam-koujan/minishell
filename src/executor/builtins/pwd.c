/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:21:26 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/19 18:22:44 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  "../../../includes/minishell.h"

void builtin_pwd()
{
    char cwd[1024];
    
    exit_stat(0, 1);
    if (getcwd(cwd, sizeof(cwd)))
    {
        ft_putstr_fd(cwd, 1);
        ft_putstr_fd("\n", 1);
    }
    else
        perror("pwd");
}
