/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:21 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:51:24 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_split_parts(const char *str)
{
	int	count;
	int	i;
	int	in_word;

	count = 0;
	i = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

char	*extract_word(const char *str, int *index)
{
	int		start;
	int		len;
	char	*word;

	while (str[*index] && str[*index] == ' ')
		(*index)++;
	start = *index;
	while (str[*index] && str[*index] != ' ')
		(*index)++;
	len = *index - start;
	if (len == 0)
		return (NULL);
	word = ft_substr(str, start, len);
	return (word);
}

char	**split_by_spaces(const char *str)
{
	char	**result;
	int		count;
	int		i;
	int		index;

	count = count_split_parts(str);
	if (count == 0)
		return (NULL);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	index = 0;
	while (i < count)
	{
		result[i] = extract_word(str, &index);
		if (!result[i])
		{
			free_char_array(result);
			return (NULL);
		}
		i++;
	}
	result[count] = NULL;
	return (result);
}
