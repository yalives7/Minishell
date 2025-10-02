/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:32 by sungor            #+#    #+#             */
/*   Updated: 2025/08/14 19:16:25 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static int	pipeline_loop(t_command *cmd, t_shell *shell,
		t_pipeline_state *state)
{
	int	pipe_fd[2];
	int	process_count;

	process_count = 0;
	while (cmd && ++process_count)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			error_exit("pipe failed");
		*(state->last_pid) = fork();
		if (*(state->last_pid) == -1)
			error_exit("fork failed");
		if (*(state->last_pid) == 0)
			handle_child_process(cmd, shell, state, pipe_fd);
		handle_parent_process(cmd, state, pipe_fd);
		cmd = cmd->next;
	}
	return (process_count);
}

static int	handle_builtin_only(t_command *commands, t_shell *shell)
{
	if (!commands->next && handle_parent_builtins(commands, shell))
	{
		update_final_underscore(commands, shell);
		close_all_heredoc_fds(commands);
		return (1);
	}
	return (0);
}

static void	setup_pipeline_state(t_pipeline_state *state, int *prev_pipe_read,
		pid_t *last_pid, t_command *commands)
{
	*prev_pipe_read = STDIN_FILENO;
	*last_pid = -1;
	state->prev_pipe_read = prev_pipe_read;
	state->last_pid = last_pid;
	state->all_cmds = commands;
}

static void	finalize_execution(t_command *commands, t_shell *shell,
		t_exec_result *result)
{
	if (result->prev_pipe_read != STDIN_FILENO)
		close(result->prev_pipe_read);
	if (result->process_count > 0)
	{
		shell->last_exit_status = wait_for_children(result->last_pid,
				result->process_count);
		if (shell->last_exit_status == 0)
			update_final_underscore(commands, shell);
	}
	close_all_heredoc_fds(commands);
	setup_signal_handlers(MAIN_SHELL);
}

void	executor(t_command *commands, t_shell *shell)
{
	int					prev_pipe_read;
	pid_t				last_pid;
	t_pipeline_state	state;
	t_exec_result		result;

	if (!commands)
		return ;
	if (process_all_heredocs(commands, shell) == -1)
		return ;
	if (handle_builtin_only(commands, shell))
		return ;
	setup_signal_handlers(EXECUTION);
	setup_pipeline_state(&state, &prev_pipe_read, &last_pid, commands);
	result.process_count = pipeline_loop(commands, shell, &state);
	result.last_pid = last_pid;
	result.prev_pipe_read = prev_pipe_read;
	finalize_execution(commands, shell, &result);
}
