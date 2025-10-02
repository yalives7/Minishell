/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:41:21 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:41:24 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(char *line)
{
	t_token	*token_list;
	int		i;
	int		processed_len;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		if (is_whitespace(line[i]))
		{
			i++;
			continue ;
		}
		processed_len = handle_metachar(&token_list, line, i);
		if (processed_len == 0)
			processed_len = handle_word(&token_list, line, i);
		i += processed_len;
	}
	add_token_back(&token_list, create_new_token(NULL, TOKEN_EOF));
	return (token_list);
}
