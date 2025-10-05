/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_word_split.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:46 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:51:50 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_arg	*create_single_arg(char *part)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(part);
	if (!arg->value)
	{
		free(arg);
		return (NULL);
	}
	arg->original_value = NULL;
	arg->quote_state = NO_QUOTE;
	return (arg);
}

static void	free_arg_array_partial(t_arg **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (args[i])
		{
			free(args[i]->value);
			free(args[i]);
		}
		i++;
	}
	free(args);
}

static t_arg	**create_arg_array(char **parts, int count)
{
	t_arg	**args;
	int		i;

	args = malloc(sizeof(t_arg *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		args[i] = create_single_arg(parts[i]);
		if (!args[i])
		{
			free_arg_array_partial(args, i);
			return (NULL);
		}
		i++;
	}
	args[count] = NULL;
	return (args);
}

t_arg	**split_expanded_value(const char *expanded_value, int *new_count)
{
	char	**parts;
	t_arg	**args;

	*new_count = 0;
	if (!expanded_value || !*expanded_value)
		return (NULL);
	parts = split_by_spaces(expanded_value);
	if (!parts)
		return (NULL);
	while (parts[*new_count])
		(*new_count)++;
	if (*new_count == 0)
	{
		free_char_array(parts);
		return (NULL);
	}
	args = create_arg_array(parts, *new_count);
	free_char_array(parts);
	return (args);
}
