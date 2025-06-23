/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:14:40 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/19 15:05:54 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

/*const char *token_type_to_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return "WORD";
	else if (type == TOKEN_PIPE)
		return "PIPE";
	else if (type == TOKEN_REDIRECT_IN)
		return "REDIRECT_IN";
	else if (type == TOKEN_REDIRECT_OUT)
		return "REDIRECT_OUT";
	else if (type == TOKEN_APPEND)
		return "APPEND";
	else if (type == TOKEN_HEREDOC)
		return "HEREDOC";
	else if (type == TOKEN_QUOTED_WORD)
		return "QUOTED_WORD";
	return "UNKNOWN";
}

void	print_tokens(t_token *token)
{
	while (token)
	{
		printf("Token: [%-12s] | Content: \"%s\"\n",
			token_type_to_str(token->type), token->content);
		token = token->next;
	}
}

void	debug_parse_quotes(char *input)
{
	int		i = 0;
	char	*word;

	printf("Debug: quote parsing for input: \"%s\"\n", input);
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else
		{
			word = parse_word_with_quotes(input, &i);
			if (!word)
			{
				printf("❌ Hata: Kapalı olmayan tırnak tespit edildi!\n");
				return ;
			}
			printf("✅ Parsed Word: \"%s\"\n", word);
			free(word);
		}
	}
}*/


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
	while (input[*i])
	{
		if (ft_isspace(input[*i]) || input[*i] == '|' || input[*i] == '<'
			|| input[*i] == '>')
			break ;
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
