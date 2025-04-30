/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:23:00 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/30 09:42:19 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_H
# define HELPER_H

# include <stdlib.h>

typedef struct s_gc
{
	void		*addr;
	struct gc	*next;
}				t_gc;

int		is_whitespace(char c);
void	*ft_malloc(size_t size, t_gc **head, int enable_free);

#endif
