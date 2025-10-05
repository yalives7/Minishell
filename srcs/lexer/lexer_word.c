/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:41:10 by msengul           #+#    #+#             */
/*   Updated: 2025/08/09 06:45:04 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_quote_logic(char *line, int *j, int *in_quote,
		char *quote_char)
{
	if (!*in_quote)
	{
		*in_quote = 1;
		*quote_char = line[*j];
	}
	else if (line[*j] == *quote_char)
	{
		*in_quote = 0;
		*quote_char = 0;
	}
	(*j)++;
	return (0);
}

static int	find_word_end(char *line, int i, int *final_in_quote,
		char *final_quote_char)
{
	int		j;
	char	quote_char;
	int		in_quote;

	j = i;
	in_quote = 0;
	quote_char = 0;
	while (line[j] && (!is_whitespace(line[j]) || in_quote)
		&& (!is_metachar(line[j]) || in_quote))
	{
		if (is_quote(line[j]))
			process_quote_logic(line, &j, &in_quote, &quote_char);
		else
			j++;
	}
	*final_in_quote = in_quote;
	*final_quote_char = quote_char;
	return (j);
}

static void	set_token_quote_state(t_token *token, int in_quote, char quote_char)
{
	if (in_quote)
	{
		if (quote_char == '\'')
			token->quote_state = SINGLE_QUOTE;
		else
			token->quote_state = DOUBLE_QUOTE;
	}
	else
		token->quote_state = NO_QUOTE;
}

int	handle_word(t_token **list, char *line, int i)
{
	int		j;
	t_token	*new_token;
	char	quote_char;
	int		in_quote;
	char	*word;

	j = find_word_end(line, i, &in_quote, &quote_char);
	word = ft_substr(line, i, j - i);
	if (!word)
		return (0);
	new_token = create_new_token(word, TOKEN_WORD);
	if (!new_token)
	{
		free(word);
		return (0);
	}
	set_token_quote_state(new_token, in_quote, quote_char);
	add_token_back(list, new_token);
	return (j - i);
}
