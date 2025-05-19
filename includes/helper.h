/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:23:00 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/19 12:04:12 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_H
# define HELPER_H

# include <stdlib.h>

typedef struct s_gc
{
	struct s_cmd_table	*parse;
	struct s_token		*tokenizer;
	struct s_env		*env;
}				t_gc;

t_gc	*init_gc(void);
void	add_to_gc(t_gc **gc, struct s_cmd_table *parse, \
	struct s_token *tokenizer, struct s_env *env);
int		is_whitespace(char c);
// void	*ft_malloc(void *ptr, t_gc **head, int enable_free);
// void	add_to_gc(t_gc **head, void	**st, int type);
// t_gc	*init_addr(void	*addr);
// void	gc_add_back(t_gc **head, t_gc *new);
void	free_arr(char	**arr);
void	free_all(t_gc **head);
void	clear_parsing(t_gc **head);
#endif
