/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:38:13 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:38:17 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	recreate_envp_without_var(t_shell *shell, int index_to_remove)
{
	char	**new_envp;
	int		env_count;
	int		i;
	int		j;

	env_count = 0;
	while (shell->envp[env_count])
		env_count++;
	new_envp = safe_malloc(sizeof(char *) * env_count);
	i = 0;
	j = 0;
	while (i < env_count)
	{
		if (i != index_to_remove)
		{
			new_envp[j] = shell->envp[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	free(shell->envp[index_to_remove]);
	free(shell->envp);
	shell->envp = new_envp;
}
