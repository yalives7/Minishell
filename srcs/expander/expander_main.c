/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:05 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:52:16 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_unquoted_segment(char **arg_ptr, t_shell *shell,
		char **result)
{
	char	*segment;
	char	*start;

	start = *arg_ptr;
	while (**arg_ptr && **arg_ptr != '\'' && **arg_ptr != '\"'
		&& **arg_ptr != '$')
		(*arg_ptr)++;
	segment = ft_substr(start, 0, *arg_ptr - start);
	if (segment)
		*result = str_append_free(*result, segment);
	if (**arg_ptr == '$')
	{
		if ((*arg_ptr)[1] == '\'' || (*arg_ptr)[1] == '\"')
			(*arg_ptr)++;
		else
		{
			segment = handle_dollar_expansion(arg_ptr, shell);
			if (segment)
				*result = str_append_free(*result, segment);
		}
	}
}

static char	*handle_unquoted(char **arg_ptr, t_shell *shell)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (**arg_ptr && **arg_ptr != '\'' && **arg_ptr != '\"')
	{
		process_unquoted_segment(arg_ptr, shell, &result);
	}
	return (result);
}

static char	*process_next_segment(char **ptr, t_shell *shell)
{
	if (**ptr == '\'')
		return (handle_single_quotes(ptr));
	else if (**ptr == '\"')
		return (handle_double_quotes(ptr, shell));
	else
		return (handle_unquoted(ptr, shell));
}

static void	handle_tilde_expansion(char **ptr, char **result, t_shell *shell)
{
	char	*home;
	char	*home_copy;
	char	*tilde;

	home = get_env_var(shell, "HOME");
	if (home)
	{
		home_copy = ft_strdup(home);
		if (home_copy)
			*result = str_append_free(*result, home_copy);
	}
	else
	{
		tilde = ft_strdup("~");
		if (tilde)
			*result = str_append_free(*result, tilde);
	}
	(*ptr)++;
}

char	*expand_argument_value(char *arg_value, t_shell *shell)
{
	char	*result;
	char	*ptr;
	char	*segment;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	ptr = arg_value;
	if (*ptr == '~' && (ptr[1] == '\0' || ptr[1] == '/'))
		handle_tilde_expansion(&ptr, &result, shell);
	while (*ptr)
	{
		segment = process_next_segment(&ptr, shell);
		if (segment)
			result = str_append_free(result, segment);
	}
	return (result);
}
