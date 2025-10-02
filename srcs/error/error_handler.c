/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:49:50 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:49:50 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	print_minishell_error(const char *context, const char *message)
{
	ft_putstr_fd("minishell: ", 2);
	if (context)
	{
		ft_putstr_fd((char *)context, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd((char *)message, 2);
}

void	print_syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token", 2);
	if (token)
	{
		ft_putstr_fd(" '", 2);
		ft_putstr_fd((char *)token, 2);
		ft_putstr_fd("'", 2);
	}
	ft_putstr_fd("\n", 2);
}

static void	handle_common_errors(t_error_type type, const char *context)
{
	if (type == ERROR_SYNTAX)
		print_syntax_error(context);
	else if (type == ERROR_COMMAND_NOT_FOUND)
		print_minishell_error(context, "command not found\n");
	else if (type == ERROR_FILE_NOT_FOUND)
		print_minishell_error(context, "No such file or directory\n");
	else if (type == ERROR_PERMISSION_DENIED)
		print_minishell_error(context, "Permission denied\n");
	else if (type == ERROR_IS_DIRECTORY)
		print_minishell_error(context, "is a directory\n");
	else if (type == ERROR_ALLOCATION)
		print_minishell_error(NULL, "Allocation Error!\n");
}

static void	handle_special_errors(t_error_type type, const char *context)
{
	if (type == ERROR_QUOTE_UNCLOSED)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote ", 2);
		if (context)
			ft_putstr_fd((char *)context, 2);
		ft_putstr_fd("\n", 2);
	}
	else if (type == ERROR_PIPE)
		print_syntax_error("|");
	else if (type == ERROR_REDIRECTION)
		print_syntax_error(NULL);
	else if (type == ERROR_SEMICOLON)
		print_syntax_error(";");
}

void	print_error(t_error_type type, const char *context)
{
	if (type <= ERROR_ALLOCATION)
		handle_common_errors(type, context);
	else if (type == ERROR_TOO_MANY_ARGS)
		print_minishell_error(context, "too many arguments\n");
	else if (type == ERROR_HOME_NOT_SET)
		print_minishell_error(context, "HOME not set\n");
	else if (type == ERROR_OLDPWD_NOT_SET)
		print_minishell_error(context, "OLDPWD not set\n");
	else if (type == ERROR_FILENAME_REQUIRED)
		print_minishell_error(context, "filename argument required\n");
	else
		handle_special_errors(type, context);
}
