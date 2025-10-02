/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:26 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:53:26 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	dispatch_parent_builtins(char *cmd_name, t_command *cmd,
		t_shell *shell)
{
	if (ft_strcmp(cmd_name, "cd") == 0)
	{
		shell->last_exit_status = builtin_cd(cmd, shell);
		return (1);
	}
	if (ft_strcmp(cmd_name, "exit") == 0)
	{
		builtin_exit(cmd, shell);
		return (1);
	}
	if (ft_strcmp(cmd_name, "export") == 0 && cmd->args[1])
	{
		shell->last_exit_status = builtin_export(cmd, shell);
		return (1);
	}
	if (ft_strcmp(cmd_name, "unset") == 0)
	{
		shell->last_exit_status = builtin_unset(cmd, shell);
		return (1);
	}
	return (0);
}

int	handle_parent_builtins(t_command *cmd, t_shell *shell)
{
	char	*cmd_name;

	if (!cmd->args || !cmd->args[0] || !cmd->args[0]->value)
		return (0);
	cmd_name = cmd->args[0]->value;
	return (dispatch_parent_builtins(cmd_name, cmd, shell));
}

void	print_error_and_exit(char *cmd, char *msg, int code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	exit(code);
}

static char	*search_in_paths(char *cmd, char **paths)
{
	char	*executable_path;
	char	*temp;
	int		i;

	i = -1;
	while (paths && paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			continue ;
		executable_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!executable_path)
			continue ;
		if (access(executable_path, X_OK) == 0)
			return (executable_path);
		free(executable_path);
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*path_var;
	char	*executable_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		return (ft_strdup(cmd));
	}
	path_var = get_env_var(shell, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	executable_path = search_in_paths(cmd, paths);
	free_char_array(paths);
	return (executable_path);
}
