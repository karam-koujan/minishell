/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:18:58 by achemlal          #+#    #+#             */
/*   Updated: 2025/04/19 18:22:58 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../../includes/minishell.h"

void builtin_env(t_simple_cmd **data, t_env *env)
{
    if((*data)->args[1])
    {
        printf("env: ‘%s’: No such file or directory\n", (*data)->args[1]);
            return ;// exit status is 127;
    }
    while (env)
    {
        if (env->value)
        {
            printf("%s", env->key);
            printf("%s\n",env->value);
        }
        env = env->next;
    }
}