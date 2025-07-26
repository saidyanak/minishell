/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:14:40 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/26 15:32:56 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
	// printf("result: %s\n", result);
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

static void	set_token_type(int quoted, t_quote_type detected,
		t_token_type *type, t_quote_type *q_type)
{
	if (quoted)
	{
		*type = TOKEN_QUOTED_WORD;
		*q_type = detected;
	}
	else
	{
		*type = TOKEN_WORD;
		*q_type = NONE_QUOTE;
	}
}

char	*parse_word_with_quotes(char *input, int *i, t_token_type *type,
		t_quote_type *q_type)
{
	char *result;
	int start;
	int quoted;
	t_quote_type detected;

	if (!input || !i || !type || !q_type)
		return (NULL);
	result = ft_strdup("");
	start = *i;
	quoted = 0;
	detected = NONE_QUOTE;
	while (input[*i] && !is_special(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quoted = 1;
			if (detected == NONE_QUOTE)
				detected = (input[*i] == '"') ? DOUBLE_QUOTE : SINGLE_QUOTE;
			result = handle_quoted_word(input, i, result);
		}
		else
			result = handle_unquoted_word(input, i, result);
		if (!result)
			return (NULL);
	}
	if (*i == start)
		return (free(result), NULL);
	set_token_type(quoted, detected, type, q_type);
	return (result);
}
