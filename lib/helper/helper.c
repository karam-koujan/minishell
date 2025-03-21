/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:21:40 by kkoujan           #+#    #+#             */
/*   Updated: 2025/03/21 20:24:07 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_escaped(char *str)
{
	int	count;

	count = 0;
	str = str + 1;
	while (*str && *str == '\\')
	{
		str = str + 1;
		count++;
	}
	return (count % 2 != 0);
}

int	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}
