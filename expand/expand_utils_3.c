/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:01:19 by syanak            #+#    #+#             */
/*   Updated: 2025/08/16 15:59:06 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

int	expand_dollar_var(char *str, int i, char **result, t_base *base)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_name = extract_var_name(str + i + 1, &var_len);
	if (var_name && var_len > 0)
	{
		var_value = find_env_value(base, var_name);
		if (var_value)
			*result = join_and_free(*result, var_value);
		free(var_name);
		return (i + var_len + 1);
	}
	if (var_name)
		free(var_name);
	*result = join_and_free(*result, ft_strdup("$"));
	return (i + 1);
}

int	is_only_empty_variable(char *str, t_base *base)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	if (!str || str[0] != '$')
		return (0);
	var_name = extract_var_name(str + 1, &var_len);
	if (!var_name)
		return (0);
	if (var_len + 1 != (int)ft_strlen(str))
	{
		free(var_name);
		return (0);
	}
	var_value = find_env_value(base, var_name);
	free(var_name);
	if (!var_value)
		return (1);
	if (ft_strlen(var_value) == 0)
	{
		free(var_value);
		return (1);
	}
	free(var_value);
	return (0);
}
