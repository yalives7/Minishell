/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:35:25 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:39:04 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	check_quote_errors(t_token *tokens)
{
	t_token	*current;
	char	quote_char;

	current = tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if ((current->quote_state == SINGLE_QUOTE
				|| current->quote_state == DOUBLE_QUOTE)
			&& current->type == TOKEN_WORD)
		{
			if (current->quote_state == SINGLE_QUOTE)
				quote_char = '\'';
			else
				quote_char = '"';
			ft_putstr_fd("minishell: syntax error: unclosed quote ", 2);
			write(2, &quote_char, 1);
			ft_putstr_fd("\n", 2);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

static int	check_pipe_start(t_token *current)
{
	if (current && current->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", 2);
		return (1);
	}
	return (0);
}

static int	check_pipe_and_redir_errors(t_token *current)
{
	while (current && current->next && current->next->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_PIPE && current->next->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token '|'\n",
				2);
			return (1);
		}
		if ((current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
				|| current->type == TOKEN_REDIR_APPEND
				|| current->type == TOKEN_REDIR_HEREDOC)
			&& (current->next->type != TOKEN_WORD))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

static int	check_pipe_end(t_token *current)
{
	if (current && current->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_syntax_errors(t_token *tokens)
{
	t_token	*current;
	t_token	*last_token;

	current = tokens;
	if (check_quote_errors(tokens) != 0)
		return (1);
	current = tokens;
	if (check_pipe_start(current) != 0)
		return (1);
	if (check_pipe_and_redir_errors(current) != 0)
		return (1);
	last_token = NULL;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_SEMICOLON)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ';'\n",
				2);
			return (1);
		}
		last_token = current;
		current = current->next;
	}
	return (check_pipe_end(last_token));
}
