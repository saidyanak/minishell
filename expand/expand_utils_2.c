/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:24:18 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 12:28:28 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_dollar_sign(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_var_char(char c, int first)
{
	if (first)
		return (ft_isalpha(c) || c == '_' || c == '?');
	return (ft_isalnum(c) || c == '_');
}

char	*extract_var_name(char *str, int *len)
{
	int	i;

	*len = 0;
	if (!str || !str[0])
		return (NULL);
	if (str[0] == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
	if (!is_valid_var_char(str[0], 1))
		return (NULL);
	i = 0;
	while (str[i] && is_valid_var_char(str[i], i == 0))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (ft_strdup(""));
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

int	has_tilde_sign(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '~' && (str[1] == 0 || str[1] == '/'))
		return (1);
	return (0);
}
