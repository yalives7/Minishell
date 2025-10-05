/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:52:50 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:52:51 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static void	print_sorted_env_vars(t_shell *shell)
{
	char	**env_copy;
	int		count;
	int		i;

	count = 0;
	while (shell->envp && shell->envp[count])
		count++;
	env_copy = ft_str_array_dup(shell->envp);
	if (!env_copy)
		return ;
	sort_str_array(env_copy, count);
	i = -1;
	while (++i < count)
	{
		if (ft_strchr(env_copy[i], '='))
			printf("%s\n", env_copy[i]);
	}
	free_char_array(env_copy);
}

int	builtin_env(t_command *cmd, t_shell *shell)
{
	if (cmd->args && cmd->args[1])
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(cmd->args[1]->value, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	if (!shell || !shell->envp)
		return (1);
	update_underscore_env("/usr/bin/env", shell);
	print_sorted_env_vars(shell);
	return (0);
}
