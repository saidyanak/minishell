/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:27:44 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 15:27:45 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static char	*process_word_characters(char *input, int *i, char *result,
		t_parse_data *data)
{
	while (input[*i] && !is_special(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			data->quoted = 1;
			if (data->detected == NONE_QUOTE)
			{
				if (input[*i] == '"')
					data->detected = DOUBLE_QUOTE;
				else
					data->detected = SINGLE_QUOTE;
			}
			result = handle_quoted_word(input, i, result);
		}
		else
			result = handle_unquoted_word(input, i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*parse_word_with_quotes(char *input, int *i, t_token_type *type,
		t_quote_type *q_type)
{
	char			*result;
	int				start;
	t_parse_data	data;

	if (!input || !i || !type || !q_type)
		return (NULL);
	result = ft_strdup("");
	start = *i;
	data.quoted = 0;
	data.detected = NONE_QUOTE;
	result = process_word_characters(input, i, result, &data);
	if (!result)
		return (NULL);
	if (*i == start)
	{
		free(result);
		free(result);
		return (NULL);
	}
	set_token_type(data.quoted, data.detected, type, q_type);
	return (result);
}

void	handle_word_or_error(char *input, int *i, t_token **head)
{
	char			*word;
	t_token_type	type;
	t_quote_type	q_type;

	word = parse_word_with_quotes(input, i, &type, &q_type);
	if (!word)
	{
		printf("minishell: syntax error: unclosed quote\n");
		if (*head)
			free_tokens(*head);
		*head = NULL;
		return ;
	}
	add_token(head, word, type, q_type);
}
