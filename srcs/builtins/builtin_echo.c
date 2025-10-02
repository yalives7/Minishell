/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:52:01 by msengul           #+#    #+#             */
/*   Updated: 2025/08/10 23:20:24 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static int	is_n_option(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	if (arg[i] == '\0' && i > 1)
		return (1);
	return (0);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline_flag;

	i = 1;
	newline_flag = 1;
	while (cmd->args[i] && cmd->args[i]->value
		&& is_n_option(cmd->args[i]->value))
	{
		newline_flag = 0;
		i++;
	}
	while (cmd->args[i] && cmd->args[i]->value)
	{
		printf("%s", cmd->args[i]->value);
		if (cmd->args[i + 1] && cmd->args[i + 1]->value)
			printf(" ");
		i++;
	}
	if (newline_flag)
		printf("\n");
	return (0);
}
