/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:38:04 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:38:05 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	find_env_var_index(char **envp, const char *name)
{
	int		i;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0
			&& (envp[i][name_len] == '=' || envp[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static char	*create_new_env_entry(char *name, char *old_val, char *val_to_add)
{
	char	*new_val;
	char	*temp;
	char	*new_entry;

	new_val = ft_strjoin(old_val, val_to_add);
	if (!new_val)
		return (NULL);
	temp = ft_strjoin(name, "=");
	if (!temp)
	{
		free(new_val);
		return (NULL);
	}
	new_entry = ft_strjoin(temp, new_val);
	free(temp);
	free(new_val);
	return (new_entry);
}

static void	handle_append_env(t_shell *shell, char *name, char *val_to_add)
{
	int		index;
	char	*old_val;
	char	*new_entry;
	char	**new_envp;

	index = find_env_var_index(shell->envp, name);
	old_val = "";
	if (index != -1 && ft_strchr(shell->envp[index], '='))
		old_val = ft_strchr(shell->envp[index], '=') + 1;
	new_entry = create_new_env_entry(name, old_val, val_to_add);
	if (!new_entry)
		return ;
	if (index != -1)
	{
		free(shell->envp[index]);
		shell->envp[index] = new_entry;
	}
	else
	{
		new_envp = ft_realloc_str_array(shell->envp, new_entry);
		if (new_envp)
			shell->envp = new_envp;
		else
			free(new_entry);
	}
}

void	update_or_add_env(t_shell *shell, const char *arg)
{
	char	*name;
	char	*value;
	int		is_append;

	is_append = 0;
	value = ft_strchr(arg, '=');
	if (value && value > arg && *(value - 1) == '+')
	{
		is_append = 1;
		name = ft_substr(arg, 0, value - arg - 1);
	}
	else if (value)
	{
		name = ft_substr(arg, 0, value - arg);
	}
	else
		name = ft_strdup(arg);
	if (!name)
		return ;
	if (is_append)
		handle_append_env(shell, name, value + 1);
	else
		set_simple_env(shell, arg, name);
	free(name);
}
