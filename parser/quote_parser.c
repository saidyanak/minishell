/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 11:38:37 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

static char	*add_char_to_result(char *result, char c)
{
	char	*temp;
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	temp = ft_strjoin(result, str);
	free(result);
	return (temp);
}

char	*handle_quoted_word(char *input, int *i, char *result)
{
	char	quote;

	quote = input[*i];
	result = add_char_to_result(result, quote);
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		result = add_char_to_result(result, input[*i]);
		(*i)++;
	}
	if (!input[*i])
		return (free(result), NULL);
	result = add_char_to_result(result, quote);
	(*i)++;
	return (result);
}

char	*handle_unquoted_word(char *input, int *i, char *result)
{
	while (input[*i] && !is_special(input[*i]) && input[*i] != '\''
		&& input[*i] != '"')
	{
		result = add_char_to_result(result, input[*i]);
		(*i)++;
	}
	return (result);
}
