/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:37 by sungor            #+#    #+#             */
/*   Updated: 2025/08/10 23:21:05 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

extern int	g_signal;

void	heredoc_read_loop(int write_fd, t_redir *redir, t_shell *shell)
{
	char	*line;

	while (1)
	{
		if (g_signal == 130)
			break ;
		line = readline("> ");
		if (!line)
		{
			if (g_signal == 130)
				break ;
			break ;
		}
		if (process_heredoc_line(line, redir, shell, write_fd))
			break ;
	}
}

void	cleanup_heredoc_child(t_shell *shell, t_command *all_cmds, int pipe_fd)
{
	int	exit_code;

	close(pipe_fd);
	free_command_list(all_cmds);
	free_char_array(shell->envp);
	if (shell->last_arg)
		free(shell->last_arg);
	exit_code = 0;
	if (g_signal == 130)
		exit_code = 130;
	exit(exit_code);
}

void	handle_heredoc_child(int *pipe_fd, t_redir *redir, t_shell *shell,
		t_command *all_cmds)
{
	g_signal = 0;
	setup_signal_handlers(HEREDOC_CHILD);
	close(pipe_fd[0]);
	heredoc_read_loop(pipe_fd[1], redir, shell);
	cleanup_heredoc_child(shell, all_cmds, pipe_fd[1]);
}

int	wait_heredoc_child(pid_t pid, int *pipe_fd)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_signal_handlers(MAIN_SHELL);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		return (-2);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		return (-2);
	}
	return (pipe_fd[0]);
}

void	close_previous_heredocs(t_command *cmds, t_command *stop_at)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = cmds;
	while (cmd && cmd != stop_at)
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
