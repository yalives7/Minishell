/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:49:54 by sungor            #+#    #+#             */
/*   Updated: 2025/08/14 20:43:40 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>

static void	setup_child_signals_and_cleanup(t_command *all_cmds, t_command *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close_unused_heredoc_fds(all_cmds, cmd);
}

static void	setup_child_fds(t_command *cmd, t_pipe_info *pipe_info, int *in_fd,
		int *out_fd)
{
	*in_fd = pipe_info->prev_read;
	*out_fd = STDOUT_FILENO;
	if (cmd->next)
	{
		close(pipe_info->pipe_fd[0]);
		*out_fd = pipe_info->pipe_fd[1];
	}
}

static void	apply_redirections_and_execute(t_command *cmd, t_shell *shell,
		t_command *all_cmds, t_fd_info *fd_info)
{
	if (fd_info->in_fd != STDIN_FILENO)
		dup2(fd_info->in_fd, STDIN_FILENO);
	if (fd_info->out_fd != STDOUT_FILENO)
		dup2(fd_info->out_fd, STDOUT_FILENO);
	if (fd_info->in_fd != STDIN_FILENO)
		close(fd_info->in_fd);
	if (fd_info->out_fd != STDOUT_FILENO)
		close(fd_info->out_fd);
	execute_command(cmd, shell, all_cmds);
}

void	child_routine(t_command *cmd, t_child_context *ctx,
		t_pipe_info *pipe_info)
{
	int			in_fd;
	int			out_fd;
	t_fd_info	fd_info;

	setup_child_signals_and_cleanup(ctx->all_cmds, cmd);
	setup_child_fds(cmd, pipe_info, &in_fd, &out_fd);
	if (process_redirections(cmd, ctx->shell, &in_fd, &out_fd) == -1)
	{
		free_command_list(ctx->all_cmds);
		free_char_array(ctx->shell->envp);
		if (ctx->shell->last_arg)
			free(ctx->shell->last_arg);
		exit(1);
	}
	fd_info.in_fd = in_fd;
	fd_info.out_fd = out_fd;
	apply_redirections_and_execute(cmd, ctx->shell, ctx->all_cmds, &fd_info);
}
