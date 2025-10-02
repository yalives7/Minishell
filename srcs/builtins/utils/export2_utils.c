/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:38:04 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:38:05 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_env_with_value(t_shell *shell, const char *arg, int index)
{
	char	*new_entry;
	char	**new_envp;

	if (index != -1)
	{
		free(shell->envp[index]);
		shell->envp[index] = ft_strdup(arg);
		if (!shell->envp[index])
			shell->envp[index] = ft_strdup("");
	}
	else
	{
		new_entry = ft_strdup(arg);
		if (new_entry)
		{
			new_envp = ft_realloc_str_array(shell->envp, new_entry);
			if (new_envp)
				shell->envp = new_envp;
			else
				free(new_entry);
		}
	}
}

static void	handle_env_without_value(t_shell *shell, const char *arg, int index)
{
	char	*new_entry;
	char	**new_envp;

	if (index == -1)
	{
		new_entry = ft_strdup(arg);
		if (new_entry)
		{
			new_envp = ft_realloc_str_array(shell->envp, new_entry);
			if (new_envp)
				shell->envp = new_envp;
			else
				free(new_entry);
		}
	}
}

void	set_simple_env(t_shell *shell, const char *arg, char *name)
{
	int		index;
	char	*value;

	index = find_env_var_index(shell->envp, name);
	value = ft_strchr(arg, '=');
	if (value)
		handle_env_with_value(shell, arg, index);
	else
		handle_env_without_value(shell, arg, index);
}
