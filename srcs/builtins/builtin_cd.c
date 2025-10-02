/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:51:47 by msengul           #+#    #+#             */
/*   Updated: 2025/08/15 01:34:48 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	perform_chdir(char *path, t_shell *shell)
{
	if (ft_strcmp(path, "") == 0)
		return (0);
	if (chdir(path) != 0)
	{
		handle_cd_error(path);
		return (1);
	}
	update_pwd_variables(shell);
	return (0);
}

static int	validate_cd_args(char **cd_args)
{
	if (cd_args[1] && cd_args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (0);
	}
	return (1);
}

static int	execute_cd_operation(t_shell *shell, char **cd_args)
{
	char	*path;
	int		status;
	int		path_needs_free;

	path_needs_free = 0;
	path = get_target_path(shell, cd_args[1], &path_needs_free);
	if (!path)
		status = 1;
	else
		status = perform_chdir(path, shell);
	if (path_needs_free)
		free(path);
	return (status);
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	**cd_args;
	int		status;

	if (!cmd || !cmd->args)
		return (1);
	cd_args = convert_t_arg_to_char_array(cmd->args);
	if (!cd_args)
		return (1);
	if (!validate_cd_args(cd_args))
		status = 1;
	else
		status = execute_cd_operation(shell, cd_args);
	free(cd_args);
	return (status);
}
