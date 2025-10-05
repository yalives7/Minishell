/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:34:03 by msengul           #+#    #+#             */
/*   Updated: 2025/08/10 23:20:17 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static char	*get_updated_shlvl(char *current_shlvl_str)
{
	int		level;
	char	*level_str;
	char	*result;

	if (!current_shlvl_str || !*current_shlvl_str)
		level = 1;
	else
	{
		level = ft_atoi(current_shlvl_str);
		if (level < 0)
			level = 0;
		level++;
	}
	level_str = ft_itoa(level);
	if (!level_str)
		return (NULL);
	result = ft_strjoin("SHLVL=", level_str);
	free(level_str);
	if (!result)
		return (NULL);
	return (result);
}

static void	process_env_var(t_shell *shell, char *env_var, int index)
{
	char	*shlvl_val;

	if (ft_strncmp(env_var, "SHLVL=", 6) == 0)
	{
		shlvl_val = env_var + 6;
		shell->envp[index] = get_updated_shlvl(shlvl_val);
		if (!shell->envp[index])
			shell->envp[index] = ft_strdup("SHLVL=1");
	}
	else
	{
		shell->envp[index] = ft_strdup(env_var);
		if (!shell->envp[index])
			shell->envp[index] = ft_strdup("");
	}
}

static void	copy_env(t_shell *shell, char **envp)
{
	int	i;
	int	count;

	count = 0;
	while (envp[count])
		count++;
	shell->envp = safe_malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		process_env_var(shell, envp[i], i);
		i++;
	}
	shell->envp[i] = NULL;
}

static void	ensure_oldpwd_exists(t_shell *shell)
{
	if (get_env_var(shell, "OLDPWD") == NULL)
		set_env_var(shell, "OLDPWD", NULL);
}

void	init_shell(t_shell *shell, char **envp)
{
	char	cwd[1024];
	int		empty_env;

	empty_env = 0;
	shell->line = NULL;
	shell->tokens = NULL;
	shell->cmds = NULL;
	shell->last_exit_status = 0;
	if (!envp || !envp[0])
		empty_env = 1;
	copy_env(shell, envp);
	if (get_env_var(shell, "PWD") == NULL && getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_var(shell, "PWD", cwd);
	if (get_env_var(shell, "SHLVL") == NULL)
		set_env_var(shell, "SHLVL", "1");
	if (!empty_env)
		ensure_oldpwd_exists(shell);
	shell->last_arg = NULL;
}
