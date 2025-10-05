/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:12 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:52:42 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_single_quotes(char **arg_ptr)
{
	char	*start;
	char	*result;
	int		len;

	(*arg_ptr)++;
	start = *arg_ptr;
	len = 0;
	while ((*arg_ptr)[len] && (*arg_ptr)[len] != '\'')
		len++;
	result = ft_substr(start, 0, len);
	if (!result)
		return (NULL);
	*arg_ptr += len;
	if (**arg_ptr == '\'')
		(*arg_ptr)++;
	return (result);
}

static void	process_double_quote_segment(char **arg_ptr, t_shell *shell,
		char **result)
{
	char	*segment;
	char	*start;

	start = *arg_ptr;
	while (**arg_ptr && **arg_ptr != '\"' && **arg_ptr != '$')
		(*arg_ptr)++;
	segment = ft_substr(start, 0, *arg_ptr - start);
	if (segment)
		*result = str_append_free(*result, segment);
	if (**arg_ptr == '$')
	{
		segment = handle_dollar_expansion(arg_ptr, shell);
		if (segment)
			*result = str_append_free(*result, segment);
	}
}

char	*handle_double_quotes(char **arg_ptr, t_shell *shell)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	(*arg_ptr)++;
	while (**arg_ptr && **arg_ptr != '\"')
		process_double_quote_segment(arg_ptr, shell, &result);
	if (**arg_ptr == '\"')
		(*arg_ptr)++;
	return (result);
}
