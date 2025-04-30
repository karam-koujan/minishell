/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:23:00 by kkoujan           #+#    #+#             */
/*   Updated: 2025/04/30 11:09:06 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_H
# define HELPER_H

# include <stdlib.h>

typedef struct s_gc
{
	void		*addr;
	struct s_gc	*next;
}				t_gc;

int		is_whitespace(char c);
void	*ft_malloc(size_t size, t_gc **head, int enable_free);
void	add_to_gc(t_gc **head, void	**st, int type);

#endif

/*

==411826==    definitely lost: 658 bytes in 11 blocks
==411826==    indirectly lost: 10,273 bytes in 286 blocks
==411826==      possibly lost: 0 bytes in 0 blocks
==411826==    still reachable: 208,233 bytes in 225 blocks
==411826==         suppressed: 0 bytes in 0 blocks

*/