/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:56:10 by msengul           #+#    #+#             */
/*   Updated: 2025/08/09 06:48:04 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	sort_str_array(char **arr, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < count)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

static void	print_env_line(char *env_str)
{
	char	*eq_ptr;

	ft_putstr_fd("declare -x ", 1);
	eq_ptr = ft_strchr(env_str, '=');
	if (eq_ptr)
	{
		write(1, env_str, eq_ptr - env_str);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(eq_ptr + 1, 1);
		ft_putstr_fd("\"", 1);
	}
	else
	{
		ft_putstr_fd(env_str, 1);
	}
	ft_putstr_fd("\n", 1);
}

static void	print_sorted_env(t_shell *shell)
{
	char	**env_copy;
	int		count;
	int		i;

	count = 0;
	while (shell->envp && shell->envp[count])
		count++;
	if (count == 0)
		return ;
	env_copy = ft_str_array_dup(shell->envp);
	if (!env_copy)
		return ;
	sort_str_array(env_copy, count);
	i = -1;
	while (++i < count)
	{
		if (ft_strncmp(env_copy[i], "_=", 2) == 0)
			continue ;
		print_env_line(env_copy[i]);
	}
	free_char_array(env_copy);
}

static char	*get_var_name_from_arg(const char *arg_val)
{
	char	*eq_pos;
	char	*name;

	eq_pos = ft_strchr(arg_val, '=');
	if (eq_pos && eq_pos > arg_val && *(eq_pos - 1) == '+')
		name = ft_substr(arg_val, 0, eq_pos - arg_val - 1);
	else if (eq_pos)
		name = ft_substr(arg_val, 0, eq_pos - arg_val);
	else
		name = ft_strdup(arg_val);
	return (name);
}

int	builtin_export(t_command *cmd, t_shell *shell)
{
	int		i;
	int		status;
	char	*name;

	status = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(shell);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		name = get_var_name_from_arg(cmd->args[i]->value);
		if (!name)
			status = 1;
		else if (!is_valid_identifier(name))
			status = export_error(cmd->args[i]->value);
		else
			update_or_add_env(shell, cmd->args[i]->value);
		if (name)
			free(name);
		i++;
	}
	return (status);
}
