/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 11:38:38 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

t_quote_type	detect_first_quote_type(char *input, int start, int end)
{
	int	i;

	i = start;
	while (i < end)
	{
		if (input[i] == '"')
			return (DOUBLE_QUOTE);
		if (input[i] == '\'')
			return (SINGLE_QUOTE);
		i++;
	}
	return (NONE_QUOTE);
}

int	is_fully_quoted(char *str, char quote_char)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if (len < 2)
		return (0);
	return (str[0] == quote_char && str[len - 1] == quote_char);
}

t_quote_type	get_quote_type(char *str)
{
	if (!str)
		return (NONE_QUOTE);
	if (is_fully_quoted(str, '"'))
		return (DOUBLE_QUOTE);
	if (is_fully_quoted(str, '\''))
		return (SINGLE_QUOTE);
	return (NONE_QUOTE);
}
