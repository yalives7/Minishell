/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:35 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:51:38 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split_args(t_arg **split_args, int count)
{
	int	j;

	j = -1;
	while (++j < count)
	{
		free(split_args[j]->value);
		free(split_args[j]);
	}
	free(split_args);
}

int	count_single_arg(t_arg *arg, t_shell *shell)
{
	char	*expanded_value;
	t_arg	**split_args;
	int		split_count;
	int		count;

	expanded_value = expand_argument_value(arg->value, shell);
	if (ft_strchr(expanded_value, ' ')
		&& !original_value_was_quoted(arg->value))
	{
		split_args = split_expanded_value(expanded_value, &split_count);
		if (split_args)
		{
			count = split_count;
			free_split_args(split_args, split_count);
		}
		else
			count = 1;
	}
	else
		count = 1;
	free(expanded_value);
	return (count);
}
