/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:52:57 by msengul           #+#    #+#             */
/*   Updated: 2025/08/14 22:02:03 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <unistd.h>

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	is_numeric_string(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (!check_overflow(str));
}

static void	cleanup_shell_on_exit(t_shell *shell)
{
	if (shell->cmds)
	{
		free_command_list(shell->cmds);
		shell->cmds = NULL;
	}
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	rl_clear_history();
	free_char_array(shell->envp);
	if (shell->last_arg)
		free(shell->last_arg);
}

static int	process_exit_args(char **args, t_shell *shell, long long *exit_code)
{
	if (!args[1])
	{
		*exit_code = shell->last_exit_status;
		return (1);
	}
	if (!is_numeric_string(args[1]))
	{
		print_numeric_error(args[1]);
		free(args);
		cleanup_shell_on_exit(shell);
		exit(2);
	}
	if (args[2])
	{
		print_too_many_args_error();
		shell->last_exit_status = 1;
		free(args);
		return (0);
	}
	*exit_code = ft_atoll(args[1]);
	return (1);
}

void	builtin_exit(t_command *cmd, t_shell *shell)
{
	char		**args;
	long long	exit_code;

	if (isatty(STDIN_FILENO))
		write(2, "exit\n", 5);
	args = convert_t_arg_to_char_array(cmd->args);
	if (!args)
	{
		cleanup_shell_on_exit(shell);
		exit(1);
	}
	if (!process_exit_args(args, shell, &exit_code))
		return ;
	free(args);
	cleanup_shell_on_exit(shell);
	exit((unsigned char)exit_code);
}
