/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:22:08 by msengul           #+#    #+#             */
/*   Updated: 2025/08/10 23:22:35 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_line(char *line, t_redir *redir, t_shell *shell,
		int write_fd)
{
	char	*expanded;

	if (ft_strcmp(line, redir->filename) == 0)
	{
		free(line);
		return (1);
	}
	if (redir->expand_heredoc)
	{
		expanded = expand_argument_value(line, shell);
		ft_putendl_fd(expanded, write_fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, write_fd);
	free(line);
	return (0);
}
