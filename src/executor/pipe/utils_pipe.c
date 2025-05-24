/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achemlal <achemlal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:22:32 by achemlal          #+#    #+#             */
/*   Updated: 2025/05/24 16:48:08 by achemlal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_dup2(int fd_src, int fd_dest, int fd_close)
{
	if (dup2(fd_src, fd_dest) == -1)
	{
		ft_close(fd_src);
		if (fd_close >= 0)
			ft_close (fd_close);
		ft_putstr_fd("Dup2 Failed\n", 2);
		exit_stat(1, 1, NULL, NULL);
	}
}

void	ft_close(int fd)
{
	if (close(fd) == -1)
	{
		ft_putstr_fd("Error closing file descriptor\n", 2);
		exit_stat(1, 1, NULL, NULL);
	}
}
