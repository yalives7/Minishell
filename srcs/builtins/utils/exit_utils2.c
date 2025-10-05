/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 23:13:21 by msengul           #+#    #+#             */
/*   Updated: 2025/08/14 23:37:05 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	char_compare(char c1, char c2)
{
	if (c1 > c2)
		return (1);
	if (c1 < c2)
		return (-1);
	return (0);
}

int	compare_digits(const char *str, int i, char *limit)
{
	int	j;
	int	cmp;

	j = 0;
	while (j < 19 && str[i + j] >= '0' && str[i + j] <= '9')
	{
		cmp = char_compare(str[i + j], limit[j]);
		if (cmp != 0)
			return (cmp);
		j++;
	}
	return (0);
}

int	compare_with_limits(const char *str, int i, int sign)
{
	char	max_pos[20];
	char	max_neg[20];

	ft_strlcpy(max_pos, "9223372036854775807", 20);
	ft_strlcpy(max_neg, "9223372036854775808", 20);
	if (sign == 1)
		return (compare_digits(str, i, max_pos) > 0);
	return (compare_digits(str, i, max_neg) > 0);
}
