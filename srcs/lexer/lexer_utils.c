/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:40:58 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:40:59 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_whitespace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

void	add_token_back(t_token **token_list, t_token *new_token)
{
	t_token	*current;

	if (!token_list || !new_token)
		return ;
	if (*token_list == NULL)
	{
		*token_list = new_token;
		return ;
	}
	current = *token_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_token;
}

t_token	*create_new_token(char *value, t_token_type token_type)
{
	t_token	*new;

	new = safe_malloc(sizeof(t_token));
	new->value = value;
	new->type = token_type;
	new->quote_state = NO_QUOTE;
	new->next = NULL;
	return (new);
}
