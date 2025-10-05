/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_structs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:39:44 by msengul           #+#    #+#             */
/*   Updated: 2025/08/08 14:39:52 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	free_token_list(t_token *list)
{
	t_token	*current;
	t_token	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

static void	free_redirs(t_redir *list)
{
	t_redir	*current;
	t_redir	*next;

	current = list;
	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_REDIR_HEREDOC && current->heredoc_fd > 0)
			close(current->heredoc_fd);
		free(current->filename);
		free(current);
		current = next;
	}
}

static void	free_args(t_arg **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]->value);
		free(args[i]->original_value);
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_command_list(t_command *list)
{
	t_command	*current;
	t_command	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free_args(current->args);
		free_redirs(current->redirs);
		free(current);
		current = next;
	}
}
