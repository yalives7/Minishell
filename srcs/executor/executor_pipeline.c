/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:13 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:53:16 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

void	handle_child_process(t_command *cmd, t_shell *shell,
		t_pipeline_state *state, int *pipe_fd)
{
	t_pipe_info		pipe_info;
	t_child_context	ctx;

	pipe_info.pipe_fd = pipe_fd;
	pipe_info.prev_read = *(state->prev_pipe_read);
	ctx.shell = shell;
	ctx.all_cmds = state->all_cmds;
	child_routine(cmd, &ctx, &pipe_info);
}

void	handle_parent_process(t_command *cmd, t_pipeline_state *state,
		int *pipe_fd)
{
	if (*(state->prev_pipe_read) != STDIN_FILENO)
		close(*(state->prev_pipe_read));
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*(state->prev_pipe_read) = pipe_fd[0];
	}
}
