/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:34:14 by msengul           #+#    #+#             */
/*   Updated: 2025/08/15 01:31:04 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	update_pwd_variables(t_shell *shell)
{
	char	*old_pwd;
	char	cwd[1024];

	old_pwd = get_env_var(shell, "PWD");
	if (old_pwd)
		set_env_var(shell, "OLDPWD", old_pwd);
	else
		set_env_var(shell, "OLDPWD", "");
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_var(shell, "PWD", cwd);
}

static char	*get_path_from_oldpwd(t_shell *shell)
{
	char	*path;

	path = get_env_var(shell, "OLDPWD");
	if (!path)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (NULL);
	}
	ft_putendl_fd(path, 1);
	return (path);
}

static char	*get_path_from_home(t_shell *shell, char *arg, int *needs_free)
{
	char	*home_dir;
	char	*path;

	*needs_free = 0;
	home_dir = get_env_var(shell, "HOME");
	if (!home_dir || *home_dir == '\0')
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (NULL);
	}
	if (!arg || ft_strcmp(arg, "~") == 0)
		return (home_dir);
	if (ft_strncmp(arg, "~/", 2) == 0)
	{
		path = ft_strjoin(home_dir, arg + 1);
		if (!path)
			return (NULL);
		*needs_free = 1;
		return (path);
	}
	return (NULL);
}

char	*get_target_path(t_shell *shell, char *arg, int *needs_free)
{
	char	*path;

	*needs_free = 0;
	if (!arg || ft_strcmp(arg, "~") == 0 || ft_strncmp(arg, "~/", 2) == 0)
	{
		path = get_path_from_home(shell, arg, needs_free);
		return (path);
	}
	if (ft_strcmp(arg, "-") == 0)
	{
		path = get_path_from_oldpwd(shell);
		return (path);
	}
	return (arg);
}
