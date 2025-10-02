/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:29 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:51:31 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	original_value_was_quoted(const char *original_value)
{
	if (!original_value)
		return (0);
	if (ft_strchr(original_value, '\'') || ft_strchr(original_value, '\"'))
		return (1);
	return (0);
}

void	copy_valid_args(t_arg **new_args, t_arg **old_args, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		if (old_args[i]->value[0] != '\0'
			|| original_value_was_quoted(old_args[i]->original_value))
			new_args[j++] = old_args[i];
		else
		{
			free(old_args[i]->value);
			free(old_args[i]->original_value);
			free(old_args[i]);
		}
		i++;
	}
	new_args[j] = NULL;
}

void	remove_empty_args(t_command *cmd)
{
	int		count;
	t_arg	**new_args;

	if (!cmd || !cmd->args)
		return ;
	count = 0;
	while (cmd->args[count])
		count++;
	new_args = safe_malloc(sizeof(t_arg *) * (count + 1));
	copy_valid_args(new_args, cmd->args, count);
	free(cmd->args);
	cmd->args = new_args;
}

int	count_total_args(t_command *cmd, t_shell *shell)
{
	int	i;
	int	total_count;

	i = -1;
	total_count = 0;
	while (cmd->args && cmd->args[++i])
		total_count += count_single_arg(cmd->args[i], shell);
	return (total_count);
}
