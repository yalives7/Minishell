/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:37:49 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:38:04 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*create_new_command(void)
{
	t_command	*cmd;

	cmd = safe_malloc(sizeof(t_command));
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

static int	parse_commands_loop(t_command *current_cmd, t_token **current_token)
{
	while (*current_token && (*current_token)->type != TOKEN_EOF)
	{
		if (parser_simple_command(current_cmd, current_token) != 0)
			return (-1);
		if (*current_token && (*current_token)->type == TOKEN_PIPE)
		{
			current_cmd->next = create_new_command();
			current_cmd = current_cmd->next;
			*current_token = (*current_token)->next;
		}
	}
	return (0);
}

t_command	*parser(t_token *tokens)
{
	t_command	*command_list;
	t_command	*current_cmd;
	t_token		*current_token;

	if (!tokens || tokens->type == TOKEN_EOF)
		return (NULL);
	if (check_syntax_errors(tokens) != 0)
	{
		g_signal = 2;
		return (NULL);
	}
	command_list = create_new_command();
	current_cmd = command_list;
	current_token = tokens;
	if (parse_commands_loop(current_cmd, &current_token) != 0)
	{
		free_command_list(command_list);
		return (NULL);
	}
	return (command_list);
}
