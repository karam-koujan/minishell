/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:23:00 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/01 10:32:12 by kkoujan          ###   ########.fr       */
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
t_gc	*init_addr(void	*addr);
void	gc_add_back(t_gc **head, t_gc *new);

#endif
