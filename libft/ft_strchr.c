/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:40:50 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:40:54 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	unsigned char	tmp;
	size_t			i;

	tmp = (unsigned char)c;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == tmp)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == tmp)
		return ((char *)&s[i]);
	return (NULL);
}
