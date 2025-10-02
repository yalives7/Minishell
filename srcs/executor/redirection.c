/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:50:53 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:54:01 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

static int	open_file(char *filename, int type)
{
	int	fd;

	fd = -1;
	if (type == TOKEN_REDIR_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == TOKEN_REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == TOKEN_REDIR_IN)
		fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	return (fd);
}

static void	update_fd(int *current_fd, int new_fd)
{
	if (*current_fd != STDIN_FILENO && *current_fd != STDOUT_FILENO)
		close(*current_fd);
	*current_fd = new_fd;
}

int	process_redirections(t_command *cmd, t_shell *shell, int *in_fd,
		int *out_fd)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_HEREDOC)
			fd = redir->heredoc_fd;
		else
			fd = open_file(redir->filename, redir->type);
		if (fd == -1)
			return (-1);
		if (fd == -2)
		{
			shell->last_exit_status = 1;
			return (-1);
		}
		if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_REDIR_HEREDOC)
			update_fd(in_fd, fd);
		else
			update_fd(out_fd, fd);
		redir = redir->next;
	}
	return (0);
}
