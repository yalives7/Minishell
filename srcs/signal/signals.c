/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:34:32 by msengul           #+#    #+#             */
/*   Updated: 2025/08/10 23:21:34 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

static void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal = 130;
}

static void	handle_sigint_execution(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	g_signal = 130;
}

static void	handle_sigquit_execution(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit (core dumped)\n", 2);
	g_signal = 131;
}

static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	g_signal = 130;
}

void	setup_signal_handlers(t_mode mode)
{
	if (mode == MAIN_SHELL)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == EXECUTION)
	{
		signal(SIGINT, handle_sigint_execution);
		signal(SIGQUIT, handle_sigquit_execution);
	}
	else if (mode == HEREDOC_PARENT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == HEREDOC_CHILD)
	{
		signal(SIGINT, handle_sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}
