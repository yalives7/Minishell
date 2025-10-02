/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:40:20 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 15:40:20 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*temp;
	int		lstsize;

	temp = lst;
	lstsize = 0;
	while (temp != NULL)
	{
		temp = temp->next;
		lstsize++;
	}
	return (lstsize);
}
