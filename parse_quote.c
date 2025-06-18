/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:14:40 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/18 12:53:39 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdlib.h>

static char	*handle_quoted_word(char *input, int *i, char *result)
{
	int		start;
	char	qoute;
	char	*tmp;

	qoute = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != qoute)
		(*i)++;
	if (!input[*i])
	{
		free(result);
		return (NULL);
	}
	tmp = ft_substr(input, start, *i - start);
	result = ft_strjoin_free(result, tmp);
	(*i)++;
	return (result);
}

static char	*handle_unquoted_word(char *input, int *i, char *result)
{
	int		start;
	char	*tmp;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '"'
		&& input[*i] != '\'')
		(*i)++;
	tmp = ft_substr(input, start, *i - start);
	return (ft_strjoin_free(result, tmp));
}

char	*parse_word_with_quotes(char *input, int *i)
{
	char	*result;

	result = ft_strdup("");
	while (input[*i] && !ft_isspace(input [*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			result = handle_quoted_word(input, i, result);
			if (!result)
				return (NULL);
		}
		else
			result = handle_unquoted_word(input, i, result);
	}
	return (result);
}
