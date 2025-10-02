/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:01 by sungor            #+#    #+#             */
/*   Updated: 2025/08/10 23:22:43 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static void	handle_special_commands(char **exec_args, t_shell *shell,
		t_command *all_cmds)
{
	if (ft_strcmp(exec_args[0], ".") == 0)
	{
		ft_putstr_fd("minishell: .", 2);
		ft_putstr_fd(": filename argument required\n", 2);
		cleanup_and_exit(exec_args, shell, all_cmds, 2);
	}
	if (ft_strcmp(exec_args[0], "..") == 0)
	{
		ft_putstr_fd("minishell: ..", 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(exec_args, shell, all_cmds, 127);
	}
}

static int	check_path_permissions(char *path, char *cmd_name)
{
	struct stat	path_stat;
	int			v;

	v = 0;
	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			v = 126;
	}
	if (v == 0 && access(path, F_OK) != 0)
		v = 127;
	if (v == 0 && access(path, X_OK) != 0)
		v = 126;
	if (v != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_name, 2);
		if (v == 127)
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": Permission denied\n", 2);
	}
	return (v);
}

static char	*get_command_path(char **exec_args, t_shell *shell,
		t_command *all_cmds)
{
	char	*path;

	path = find_command_path(exec_args[0], shell);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(exec_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(exec_args, shell, all_cmds, 127);
	}
	return (path);
}

static void	execute_with_path(char **exec_args, char *path, t_shell *shell,
		t_command *all_cmds)
{
	int	v;

	if (ft_strchr(exec_args[0], '/'))
	{
		v = check_path_permissions(path, exec_args[0]);
		if (v != 0)
		{
			free(path);
			cleanup_and_exit(exec_args, shell, all_cmds, v);
		}
	}
	update_underscore_env(path, shell);
	execve(path, exec_args, shell->envp);
	perror("execve");
	free(path);
	cleanup_and_exit(exec_args, shell, all_cmds, 126);
}

void	execute_command(t_command *cmd, t_shell *shell, t_command *all_cmds)
{
	char	**exec_args;
	char	*path;
	int		exit_status;

	exec_args = convert_t_arg_to_char_array(cmd->args);
	if (!exec_args || !exec_args[0])
		cleanup_and_exit(exec_args, shell, all_cmds, 0);
	if (dispatch_builtin(cmd, shell) != -1)
	{
		exit_status = shell->last_exit_status;
		cleanup_and_exit(exec_args, shell, all_cmds, exit_status);
	}
	handle_special_commands(exec_args, shell, all_cmds);
	path = get_command_path(exec_args, shell, all_cmds);
	execute_with_path(exec_args, path, shell, all_cmds);
}
