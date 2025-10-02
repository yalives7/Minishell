/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:39:01 by msengul           #+#    #+#             */
/*   Updated: 2025/08/14 23:45:54 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define INT_MAX 2147483647

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	error_exit(char *error)
{
	printf("%s\n", error);
	return (1);
}

long	ft_atol(char *str)
{
	long	result;

	result = 0;
	while (is_space(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			error_exit("Give a + number.");
		str++;
	}
	if (!is_digit(*str))
		error_exit("Error: Invalid character found.");
	while (is_digit(*str))
	{
		result = (result * 10) + (*str - '0');
		if (result > INT_MAX)
			error_exit("Give an int number");
		str++;
	}
	while (is_space(*str))
		str++;
	if (*str != '\0')
		error_exit("Error: Trailing characters found.");
	return (result);
}
