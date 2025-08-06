/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:01:19 by syanak            #+#    #+#             */
/*   Updated: 2025/08/05 16:02:08 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_only_empty_variable(char *str, t_base *base)
{
	char *var_name;
	char *var_value;
	int var_len;

	if (!str || str[0] != '$')
		return (0);

	var_name = extract_var_name(str + 1, &var_len);
	if (!var_name)
		return (0);
	if (var_len + 1 != (int)strlen(str))
	{
		free(var_name);
		return (0);
	}

	var_value = find_env_value(base, var_name);
	free(var_name);

	if (!var_value) // Değişken yok
		return (1);
	if (strlen(var_value) == 0) // Değişken boş
	{
		free(var_value);
		return (1);
	}
	free(var_value);
	return (0);
}