/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:18:25 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/24 12:18:34 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

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
		printf("[%d] Type: %-12s Content: '%s'\n", 
			i, get_token_type_name(token->type), token->content);
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
	result = parse_word_with_quotes(input, &i, &type);
	
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