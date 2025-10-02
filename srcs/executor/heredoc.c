/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:46 by sungor            #+#    #+#             */
/*   Updated: 2025/08/14 19:59:54 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

extern int	g_signal;

static int	process_single_heredoc(t_redir *redir, t_shell *shell,
		t_command *cmds)
{
	int	fd;

	fd = handle_heredoc(redir, shell, cmds);
	if (fd == -2)
	{
		close_previous_heredocs(cmds, NULL);
		shell->last_exit_status = 130;
		return (-1);
	}
	redir->heredoc_fd = fd;
	return (0);
}

static int	process_command_heredocs(t_command *cmd, t_shell *shell,
		t_command *cmds)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_HEREDOC)
		{
			if (process_single_heredoc(redir, shell, cmds) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	process_all_heredocs(t_command *cmds, t_shell *shell)
{
	t_command	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (process_command_heredocs(cmd, shell, cmds) == -1)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}

int	handle_heredoc(t_redir *redir, t_shell *shell, t_command *all_cmds)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		error_exit("pipe failed for heredoc");
	setup_signal_handlers(HEREDOC_PARENT);
	pid = fork();
	if (pid == -1)
		error_exit("fork failed for heredoc");
	if (pid == 0)
		handle_heredoc_child(pipe_fd, redir, shell, all_cmds);
	return (wait_heredoc_child(pid, pipe_fd));
}
