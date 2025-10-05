/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:20 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:53:22 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

void	cleanup_and_exit(char **exec_args, t_shell *shell, t_command *all_cmds,
		int exit_code)
{
	if (exec_args)
		free(exec_args);
	free_command_list(all_cmds);
	free_char_array(shell->envp);
	if (shell->last_arg)
		free(shell->last_arg);
	exit(exit_code);
}

void	close_all_heredoc_fds(t_command *commands)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_REDIR_HEREDOC && redir->heredoc_fd > 0)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	close_unused_heredoc_fds(t_command *commands, t_command *current)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = commands;
	while (cmd)
	{
		if (cmd != current)
		{
			redir = cmd->redirs;
			while (redir)
			{
				if (redir->type == TOKEN_REDIR_HEREDOC && redir->heredoc_fd > 0)
				{
					close(redir->heredoc_fd);
				}
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
}

int	wait_for_children(pid_t last_pid, int process_count)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < process_count)
	{
		if (wait(&status) == last_pid)
			last_status = status;
		i++;
	}
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (last_status);
}

void	update_final_underscore(t_command *commands, t_shell *shell)
{
	t_command	*last_cmd;
	char		*path;

	last_cmd = commands;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
	if (!last_cmd || !last_cmd->args || !last_cmd->args[0])
		return ;
	path = find_command_path(last_cmd->args[0]->value, shell);
	if (path)
	{
		update_underscore_env(path, shell);
		free(path);
	}
	else if (last_cmd->args[0]->value)
	{
		update_underscore_env(last_cmd->args[0]->value, shell);
	}
}
