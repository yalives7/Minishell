/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:34:10 by msengul           #+#    #+#             */
/*   Updated: 2025/08/15 01:35:48 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

int			g_signal = 0;

static int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (ft_isspace(line[i]))
		i++;
	return (line[i] == '\0');
}

static int	process_tokens_and_commands(char *line, t_shell *shell,
		t_token **tokens, t_command **commands)
{
	add_history(line);
	*tokens = lexer(line);
	if (!*tokens)
	{
		free(line);
		return (0);
	}
	*commands = parser(*tokens);
	free_token_list(*tokens);
	if (!*commands)
	{
		free(line);
		shell->last_exit_status = 2;
		return (0);
	}
	return (1);
}

int	execute_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	if (is_empty_line(line))
	{
		free(line);
		return (shell->last_exit_status);
	}
	if (!process_tokens_and_commands(line, shell, &tokens, &commands))
		return (shell->last_exit_status);
	shell->line = line;
	shell->cmds = commands;
	expander(commands, shell);
	executor(commands, shell);
	shell->line = NULL;
	shell->cmds = NULL;
	free_command_list(commands);
	free(line);
	return (shell->last_exit_status);
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	setup_signal_handlers(MAIN_SHELL);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (g_signal == 130)
		{
			shell->last_exit_status = 130;
			g_signal = 0;
		}
		if (*line)
		{
			execute_line(line, shell);
		}
		else
		{
			free(line);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	struct termios	original_termios;

	(void)argc;
	(void)argv;
	if (isatty(STDIN_FILENO))
		tcgetattr(STDIN_FILENO, &original_termios);
	init_shell(&shell, envp);
	shell_loop(&shell);
	if (isatty(STDIN_FILENO))
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
	rl_clear_history();
	free_char_array(shell.envp);
	if (shell.last_arg)
		free(shell.last_arg);
	return (shell.last_exit_status);
}
