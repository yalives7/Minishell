/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:00:31 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:46:37 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	is_separator(char c, char custom_sep)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == custom_sep || c == '\0')
		return (1);
	return (0);
}

int	word_count(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (is_separator(*str, c))
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static int	safe_malloc_s(char **array, int position, size_t buffer)
{
	int	i;

	i = 0;
	array[position] = malloc(buffer);
	if (NULL == array[position])
	{
		while (i < position)
			free(array[i++]);
		free(array);
		return (1);
	}
	return (0);
}

int	cpy_str(char **array, char const *s, char c)
{
	int	len;
	int	i;
	int	k;

	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (!*s)
			break ;
		len = 0;
		while (s[len] != c && s[len] != '\0')
			len++;
		if (safe_malloc_s(array, i, len + 1))
			return (1);
		k = -1;
		while (++k < len)
			array[i][k] = s[k];
		array[i][k] = '\0';
		i++;
		s += len;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	size_t	word_c;
	char	**array;

	if (NULL == s)
		return (NULL);
	word_c = word_count(s, c);
	array = malloc((word_c + 1) * sizeof(char *));
	if (NULL == array)
		return (NULL);
	if (cpy_str(array, s, c))
		return (NULL);
	array[word_c] = NULL;
	return (array);
}
