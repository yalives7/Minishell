/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:37:41 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 16:37:45 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	error_exit(const char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

void	safe_exit(int exit_code)
{
	exit(exit_code);
}

void	print_builtin_error(const char *builtin, const char *arg,
		const char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)builtin, 2);
	ft_putstr_fd(": ", 2);
	if (arg)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putstr_fd("': ", 2);
	}
	ft_putstr_fd((char *)message, 2);
	ft_putstr_fd("\n", 2);
}

void	error_exit_with_type(t_error_type type, const char *context,
		int exit_code)
{
	print_error(type, context);
	exit(exit_code);
}
