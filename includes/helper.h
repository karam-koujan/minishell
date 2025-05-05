/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:23:00 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/05 20:28:19 by kkoujan          ###   ########.fr       */
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
void	*ft_malloc(void *ptr, t_gc **head, int enable_free);
void	add_to_gc(t_gc **head, void	**st, int type);
t_gc	*init_addr(void	*addr);
void	gc_add_back(t_gc **head, t_gc *new);
void	free_arr(char	**arr);
void	free_all(t_gc **head);
#endif
