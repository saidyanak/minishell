/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:14:40 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/01 13:03:51 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

char	*handle_quoted_word(char *input, int *i, char *result)
{
	char	quote;
	char	*tmp;
	int		start;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
		return (free(result), NULL);
	tmp = ft_substr(input, start, *i - start);
	if (!tmp)
		return (free(result), NULL);
	result = ft_strjoin_free(result, tmp);
	(*i)++;
	return (result);	
}

char	*handle_unquoted_word(char *input, int *i, char *result)
{
	int		start;
	char	*tmp;

	start = *i;
	while (input[*i]
		&& !is_special(input[*i])
		&& input[*i] != '\''
		&& input[*i] != '"')
		(*i)++;
	if (*i == start)
		return (result);
	tmp = ft_substr(input, start, *i - start);
	if (!tmp)
		return (free(result), NULL);
	return (ft_strjoin_free(result, tmp));
}

char	*parse_word_with_quotes(char *input, int *i, t_token_type *type)
{
	char	*result;
	int		start;
	int		quoted;

	if (!input || !i || !type)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	start = *i;
	quoted = 0;
	while (input[*i] && !is_special(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quoted = 1;
			result = handle_quoted_word(input, i, result);
		}
		else
			result = handle_unquoted_word(input, i, result);
		if (!result)
			return (NULL);
	}
	if (*i == start)
		return (free(result), NULL);
	if (quoted)
		*type = TOKEN_QUOTED_WORD;
	else
		*type = TOKEN_WORD;
	return (result);
}

