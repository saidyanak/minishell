/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:18:25 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/11 12:59:57 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static const char	*get_quote_type_name(t_quote_type q_type)
{
	switch (q_type)
	{
		case DOUBLE_QUOTE:
			return ("DOUBLE_QUOTE");
		case SINGLE_QUOTE:
			return ("SINGLE_QUOTE");
		case NONE_QUOTE:
			return ("NONE_QUOTE");
		default:
			return ("UNKNOWN_QUOTE");
	}
}

static const char	*get_token_type_name(t_token_type type)
{
	switch (type)
	{
		case TOKEN_WORD:
			return ("WORD");
		case TOKEN_QUOTED_WORD:
			return ("QUOTED_WORD");
		case TOKEN_PIPE:
			return ("PIPE");
		case TOKEN_REDIRECT_IN:
			return ("REDIRECT_IN");
		case TOKEN_REDIRECT_OUT:
			return ("REDIRECT_OUT");
		case TOKEN_APPEND:
			return ("APPEND");
		case TOKEN_HEREDOC:
			return ("HEREDOC");
		default:
			return ("UNKNOWN");
	}
}

void	print_tokens(t_token *token)
{
	int	i;

	if (!token)
	{
		printf("No tokens found.\n");
		return;
	}
	
	printf("\n=== TOKENS ===\n");
	i = 0;
	while (token)
	{
		printf("[%d] Type: %-12s Quote: %-12s Content: '%s'\n", 
			i, get_token_type_name(token->type), 
			get_quote_type_name(token->q_type), token->content);
		token = token->next;
		i++;
	}
	printf("==============\n\n");
}

void	print_env(t_env *env)
{
	if (!env)
	{
		printf("No environment variables found.\n");
		return;
	}
	
	printf("\n=== ENVIRONMENT ===\n");
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	printf("===================\n\n");
}

void	debug_parse_quotes(char *input)
{
	int				i;
	t_token_type	type;
	char			*result;

	if (!input)
		return;
	
	printf("DEBUG: Parsing '%s'\n", input);
	i = 0;
	result = parse_word_with_quotes(input, &i, &type, NULL);
	
	if (result)
	{
		printf("Result: '%s', Type: %s, Position: %d\n", 
			result, get_token_type_name(type), i);
		free(result);
	}
	else
	{
		printf("Parse failed at position %d\n", i);
	}
}

void	debug_token_with_quotes(t_token *token)
{
	if (!token)
	{
		printf("No token provided for quote debug.\n");
		return;
	}
	
	printf("\n=== TOKEN QUOTE DEBUG ===\n");
	printf("Content: '%s'\n", token->content);
	printf("Token Type: %s\n", get_token_type_name(token->type));
	printf("Quote Type: %s\n", get_quote_type_name(token->q_type));
	printf("=========================\n\n");
}