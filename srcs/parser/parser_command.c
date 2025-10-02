/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:35:02 by msengul           #+#    #+#             */
/*   Updated: 2025/08/13 22:20:25 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_arg	*create_new_arg(t_token *token)
{
	t_arg	*new_arg;

	new_arg = safe_malloc(sizeof(t_arg));
	new_arg->value = ft_strdup(token->value);
	if (!new_arg->value)
		return (NULL);
	new_arg->original_value = ft_strdup(token->value);
	if (!new_arg->original_value)
	{
		free(new_arg->value);
		free(new_arg);
		return (NULL);
	}
	new_arg->quote_state = token->quote_state;
	return (new_arg);
}

static int	process_word_token(t_command *cmd, t_token **token_stream, int *i)
{
	t_arg	*new_arg;

	new_arg = create_new_arg(*token_stream);
	if (!new_arg)
		return (-1);
	cmd->args[*i] = new_arg;
	(*i)++;
	*token_stream = (*token_stream)->next;
	return (0);
}

static void	init_args_null(t_command *cmd, int arg_count)
{
	int	j;

	j = 0;
	while (j <= arg_count)
	{
		cmd->args[j] = NULL;
		j++;
	}
}

int	parser_simple_command(t_command *cmd, t_token **token_stream)
{
	int	arg_count;
	int	i;

	arg_count = count_args(*token_stream);
	cmd->args = safe_malloc(sizeof(t_arg *) * (arg_count + 1));
	init_args_null(cmd, arg_count);
	i = 0;
	while (*token_stream && (*token_stream)->type != TOKEN_PIPE
		&& (*token_stream)->type != TOKEN_EOF)
	{
		if ((*token_stream)->type > TOKEN_PIPE)
		{
			if (handle_redirection(cmd, token_stream) != 0)
				return (-1);
		}
		else
		{
			if (process_word_token(cmd, token_stream, &i) != 0)
				return (-1);
		}
	}
	return (0);
}

int	count_args(t_token *token)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = token;
	while (temp && temp->type != TOKEN_PIPE && temp->type != TOKEN_EOF)
	{
		if (temp->type > TOKEN_PIPE)
		{
			temp = temp->next;
			if (temp)
				temp = temp->next;
		}
		else
		{
			i++;
			temp = temp->next;
		}
	}
	return (i);
}
