/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:39:54 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:40:14 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**ft_str_array_dup(char **array)
{
	char	**new_array;
	int		i;
	int		count;

	if (!array)
		return (NULL);
	count = 0;
	while (array[count])
		count++;
	new_array = malloc(sizeof(char *) * (count + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_char_array(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

char	**ft_realloc_str_array(char **old_array, char *new_str)
{
	int		i;
	char	**new_array;

	i = 0;
	while (old_array && old_array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (old_array && old_array[i])
	{
		new_array[i] = old_array[i];
		i++;
	}
	new_array[i++] = new_str;
	new_array[i] = NULL;
	if (old_array)
		free(old_array);
	return (new_array);
}

char	**convert_t_arg_to_char_array(t_arg **args)
{
	char	**result;
	int		count;
	int		i;

	if (!args)
		return (NULL);
	count = 0;
	while (args[count])
		count++;
	result = safe_malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		result[i] = args[i]->value;
		i++;
	}
	result[i] = NULL;
	return (result);
}
