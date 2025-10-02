/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_metachar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:40:47 by msengul           #+#    #+#             */
/*   Updated: 2025/08/09 06:45:36 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_output_redir(t_token **list, char *line, int i)
{
	char	*token_val;

	if (line[i + 1] == '>')
	{
		token_val = ft_strdup(">>");
		if (token_val)
			add_token_back(list, create_new_token(token_val,
					TOKEN_REDIR_APPEND));
		return (2);
	}
	token_val = ft_strdup(">");
	if (token_val)
		add_token_back(list, create_new_token(token_val, TOKEN_REDIR_OUT));
	return (1);
}

static int	handle_input_redir(t_token **list, char *line, int i)
{
	char	*token_val;

	if (line[i + 1] == '<')
	{
		token_val = ft_strdup("<<");
		if (token_val)
			add_token_back(list, create_new_token(token_val,
					TOKEN_REDIR_HEREDOC));
		return (2);
	}
	token_val = ft_strdup("<");
	if (token_val)
		add_token_back(list, create_new_token(token_val, TOKEN_REDIR_IN));
	return (1);
}

static int	handle_pipe_token(t_token **list)
{
	char	*token_val;

	token_val = ft_strdup("|");
	if (token_val)
		add_token_back(list, create_new_token(token_val, TOKEN_PIPE));
	return (1);
}

int	handle_metachar(t_token **list, char *line, int i)
{
	char	*token_val;

	if (line[i] == '>')
		return (handle_output_redir(list, line, i));
	if (line[i] == '<')
		return (handle_input_redir(list, line, i));
	if (line[i] == '|')
		return (handle_pipe_token(list));
	if (line[i] == ';')
	{
		token_val = ft_strdup(";");
		if (token_val)
			add_token_back(list, create_new_token(token_val, TOKEN_SEMICOLON));
		return (1);
	}
	return (0);
}
