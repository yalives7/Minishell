/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:40:22 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:40:27 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t bytes)
{
	void	*new;

	new = malloc(bytes);
	if (!new)
		error_exit("Allocation Error!\n");
	return (new);
}

char	*str_append_free(char *s1, char *s2)
{
	char	*new_str;

	if (!s2)
		return (s1);
	if (!s1)
		return (s2);
	new_str = ft_strjoin(s1, s2);
	if (!new_str)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	free(s1);
	free(s2);
	return (new_str);
}
