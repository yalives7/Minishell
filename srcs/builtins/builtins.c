/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:34:08 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:34:09 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dispatch_builtin(t_command *cmd, t_shell *shell)
{
	char	*name;

	if (!cmd->args || !cmd->args[0] || !cmd->args[0]->value)
		return (-1);
	name = cmd->args[0]->value;
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(cmd, shell));
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export(cmd, shell));
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(cmd, shell));
	if (ft_strcmp(name, "exit") == 0)
	{
		builtin_exit(cmd, shell);
		return (0);
	}
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(cmd, shell));
	return (-1);
}
