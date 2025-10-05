/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:34:30 by msengul           #+#    #+#             */
/*   Updated: 2025/08/09 06:46:49 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_underscore_env(char *last_arg, t_shell *shell)
{
	int		i;
	char	*new_underscore;

	if (!last_arg)
		return ;
	new_underscore = ft_strjoin("_=", last_arg);
	if (!new_underscore)
		return ;
	i = 0;
	while (shell->envp && shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "_=", 2) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = new_underscore;
			return ;
		}
		i++;
	}
	shell->envp = ft_realloc_str_array(shell->envp, new_underscore);
}

static char	*create_var_string(const char *name, const char *value)
{
	char	*var_name_part;
	char	*new_var;

	var_name_part = ft_strjoin(name, "=");
	if (!var_name_part)
		return (NULL);
	if (!value)
		new_var = ft_strdup(var_name_part);
	else
	{
		new_var = ft_strjoin(var_name_part, value);
		if (!new_var)
			return (NULL);
	}
	free(var_name_part);
	return (new_var);
}

static void	add_new_env_var(t_shell *shell, char *new_var)
{
	int		count;
	char	**new_envp;
	int		i;

	count = 0;
	while (shell->envp && shell->envp[count])
		count++;
	new_envp = safe_malloc(sizeof(char *) * (count + 2));
	i = 0;
	while (i < count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	if (shell->envp)
		free(shell->envp);
	shell->envp = new_envp;
}

void	set_env_var(t_shell *shell, const char *name, const char *value)
{
	int		index;
	char	*new_var;

	new_var = create_var_string(name, value);
	if (!new_var)
		return ;
	index = find_env_var_index(shell->envp, name);
	if (index != -1)
	{
		free(shell->envp[index]);
		shell->envp[index] = new_var;
	}
	else
		add_new_env_var(shell, new_var);
}

char	*get_env_var(t_shell *shell, const char *name)
{
	int	i;
	int	len;

	if (!shell || !shell->envp || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
