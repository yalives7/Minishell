/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungor <sungor@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:51:00 by sungor            #+#    #+#             */
/*   Updated: 2025/08/09 06:52:33 by sungor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_val_copy(t_shell *shell, const char *name)
{
	char	*value;
	char	*result;

	value = get_env_var(shell, name);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	if (!result)
		result = ft_strdup("");
	return (result);
}

static char	*expand_regular_var(char **arg_ptr, t_shell *shell)
{
	char	*var_name;
	char	*value;
	int		i;

	i = 0;
	while ((*arg_ptr)[i] && (ft_isalnum((*arg_ptr)[i]) || (*arg_ptr)[i] == '_'))
		i++;
	var_name = ft_substr(*arg_ptr, 0, i);
	if (!var_name)
	{
		value = ft_strdup("");
		if (!value)
			return (NULL);
		return (value);
	}
	*arg_ptr += i;
	value = get_env_val_copy(shell, var_name);
	free(var_name);
	return (value);
}

char	*handle_dollar_expansion(char **arg_ptr, t_shell *shell)
{
	char	*result;

	(*arg_ptr)++;
	if (**arg_ptr == '?')
	{
		(*arg_ptr)++;
		result = ft_itoa(shell->last_exit_status);
		if (!result)
			result = ft_strdup("0");
		return (result);
	}
	if (ft_isdigit(**arg_ptr))
	{
		(*arg_ptr)++;
		result = ft_strdup("");
		return (result);
	}
	if (!ft_isalpha(**arg_ptr) && **arg_ptr != '_')
	{
		result = ft_strdup("$");
		return (result);
	}
	return (expand_regular_var(arg_ptr, shell));
}
