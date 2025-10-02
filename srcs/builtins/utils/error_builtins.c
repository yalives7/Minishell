/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:33:43 by msengul           #+#    #+#             */
/*   Updated: 2025/08/09 06:47:38 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

int	export_error(const char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

void	print_numeric_error(char *arg_val)
{
	write(2, "minishell: exit: ", 17);
	write(2, arg_val, ft_strlen(arg_val));
	write(2, ": numeric argument required\n", 28);
}

void	print_too_many_args_error(void)
{
	write(2, "minishell: exit: too many arguments\n", 36);
}

void	handle_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

int	handle_invalid_option(const char *option)
{
	ft_putstr_fd("minishell: unset: ", 2);
	ft_putstr_fd((char *)option, 2);
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("unset: usage: unset [name ...]\n", 2);
	return (2);
}
