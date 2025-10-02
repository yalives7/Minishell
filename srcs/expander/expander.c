/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:58 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:52:02 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_expanded_arg(t_command *cmd, int i, t_shell *shell,
		t_arg_builder *builder)
{
	char	*expanded_value;
	t_arg	**split_args;
	int		split_count;
	int		j;

	expanded_value = expand_argument_value(cmd->args[i]->value, shell);
	if (ft_strchr(expanded_value, ' ')
		&& !original_value_was_quoted(cmd->args[i]->value))
	{
		split_args = split_expanded_value(expanded_value, &split_count);
		if (split_args)
		{
			j = -1;
			while (++j < split_count)
				(*builder->new_args)[(*builder->k)++] = split_args[j];
			free(split_args);
			free(expanded_value);
			return (split_count);
		}
	}
	free(cmd->args[i]->value);
	cmd->args[i]->value = expanded_value;
	(*builder->new_args)[(*builder->k)++] = cmd->args[i];
	return (1);
}

static void	expand_args_with_splitting(t_command *cmd, t_shell *shell)
{
	t_arg			**new_args_array;
	int				total_count;
	int				i;
	int				k;
	t_arg_builder	builder;

	total_count = count_total_args(cmd, shell);
	new_args_array = malloc(sizeof(t_arg *) * (total_count + 1));
	if (!new_args_array)
		return ;
	i = -1;
	k = 0;
	builder.new_args = &new_args_array;
	builder.k = &k;
	while (cmd->args && cmd->args[++i])
		process_expanded_arg(cmd, i, shell, &builder);
	new_args_array[total_count] = NULL;
	free(cmd->args);
	cmd->args = new_args_array;
}

static void	expand_redirections(t_command *cmd, t_shell *shell)
{
	t_redir	*redir;
	char	*expanded_value;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type != TOKEN_REDIR_HEREDOC)
		{
			expanded_value = expand_argument_value(redir->filename, shell);
			free(redir->filename);
			redir->filename = expanded_value;
		}
		redir = redir->next;
	}
}

static void	expand_single_command(t_command *cmd, t_shell *shell)
{
	expand_args_with_splitting(cmd, shell);
	remove_empty_args(cmd);
	expand_redirections(cmd, shell);
}

void	expander(t_command *commands, t_shell *shell)
{
	t_command	*cmd;

	cmd = commands;
	while (cmd)
	{
		expand_single_command(cmd, shell);
		cmd = cmd->next;
	}
}
