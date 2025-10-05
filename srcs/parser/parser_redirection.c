/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:35:16 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:38:48 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_redir_to_command(t_command *cmd, t_redir *new_redir)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = new_redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static int	setup_heredoc_redir(t_redir *redir, char *delimiter_token_value)
{
	int	len;

	len = ft_strlen(delimiter_token_value);
	if (len >= 2 && (delimiter_token_value[0] == '\''
			|| delimiter_token_value[0] == '\"')
		&& (delimiter_token_value[0] == delimiter_token_value[len - 1]))
	{
		redir->expand_heredoc = 0;
		redir->filename = ft_substr(delimiter_token_value, 1, len - 2);
		if (!redir->filename)
			return (-1);
	}
	else
	{
		redir->filename = ft_strdup(delimiter_token_value);
		if (!redir->filename)
			return (-1);
	}
	return (0);
}

static int	validate_and_setup_redir(t_redir *redir, t_token *token)
{
	if (!token || token->type != TOKEN_WORD)
	{
		free(redir);
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		g_signal = 2;
		return (-1);
	}
	if (redir->type == TOKEN_REDIR_HEREDOC)
		setup_heredoc_redir(redir, token->value);
	else
	{
		redir->filename = ft_strdup(token->value);
		if (!redir->filename)
			return (-1);
	}
	return (0);
}

int	handle_redirection(t_command *cmd, t_token **current_token)
{
	t_redir	*redir;
	t_token	*token;

	redir = safe_malloc(sizeof(t_redir));
	redir->type = (*current_token)->type;
	redir->next = NULL;
	redir->expand_heredoc = 1;
	redir->heredoc_fd = -1;
	token = (*current_token)->next;
	if (validate_and_setup_redir(redir, token) != 0)
		return (-1);
	add_redir_to_command(cmd, redir);
	*current_token = token->next;
	return (0);
}
