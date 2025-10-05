/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:33:00 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:33:40 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_unset_arg(t_shell *shell, const char *arg)
{
	int	index;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	index = find_env_var_index(shell->envp, arg);
	if (index != -1)
	{
		recreate_envp_without_var(shell, index);
	}
	return (0);
}

int	builtin_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	final_status;
	int	options_ended;

	i = 1;
	final_status = 0;
	options_ended = 0;
	if (!cmd->args || !cmd->args[i])
		return (0);
	while (cmd->args[i])
	{
		if (!options_ended && ft_strcmp(cmd->args[i]->value, "--") == 0)
			options_ended = 1;
		else if (!options_ended && cmd->args[i]->value[0] == '-')
		{
			return (handle_invalid_option(cmd->args[i]->value));
		}
		else if (handle_unset_arg(shell, cmd->args[i]->value) != 0)
			final_status = 1;
		i++;
	}
	return (final_status);
}
