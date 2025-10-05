/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 22:00:21 by msengul           #+#    #+#             */
/*   Updated: 2025/08/14 23:41:04 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_leading_zeros(const char *str, int i)
{
	while (str[i] == '0' && str[i + 1] != '\0')
		i++;
	return (i);
}

static int	get_digit_len(const char *str, int i)
{
	int	len;

	len = 0;
	while (str[i + len] >= '0' && str[i + len] <= '9')
		len++;
	return (len);
}

int	check_overflow(const char *str)
{
	int	i;
	int	sign;
	int	len;

	i = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	i = skip_leading_zeros(str, i);
	len = get_digit_len(str, i);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	return (compare_with_limits(str, i, sign));
}
