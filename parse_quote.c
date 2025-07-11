/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:14:40 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/11 12:59:55 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

char	*handle_quoted_word(char *input, int *i, char *result, t_gc *gc)
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
		return (NULL);
	tmp = ft_substr_gc(gc, input, start, *i - start);
	if (!tmp)
		return (NULL);
	result = ft_strjoin_free_gc(result, tmp, gc);
	(*i)++;
	return (result);	
}

char	*handle_unquoted_word(char *input, int *i, char *result, t_gc *gc)
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
	tmp = ft_substr_gc(gc, input, start, *i - start);
	if (!tmp)
		return (NULL);
	return (ft_strjoin_free_gc(result, tmp, gc));
}

char	*parse_word_with_quotes(char *input, int *i, t_token_type *type, t_gc *gc)
{
	char	*result;
	int		start;
	int		quoted;

	if (!input || !i || !type)
		return (NULL);
	result = ft_strdup_gc(gc, "");
	if (!result)
		return (NULL);
	start = *i;
	quoted = 0;
	while (input[*i] && !is_special(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quoted = 1;
			result = handle_quoted_word(input, i, result, gc);
		}
		else
			result = handle_unquoted_word(input, i, result, gc);
		if (!result)
			return (NULL);
	}
	if (*i == start)
		return (NULL);
	if (quoted)
		*type = TOKEN_QUOTED_WORD;
	else
		*type = TOKEN_WORD;
	return (result);
}

