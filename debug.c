/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:18:25 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/07 02:38:29 by syanak           ###   ########.fr       */
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
static const char	*get_token_quote_name(t_quote_type type)
{
	switch (type)
	{
	case DOUBLE_QUOTE:
		return ("DOUBLE_QUOTE");
	case SINGLE_QUOTE:
		return ("SINGLE_QUOTE");
	case NONE_QUOTE:
		return ("NONE_QUOTE");
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
		return ;
	}
	printf("\n=== TOKENS ===\n");
	i = 0;
	while (token)
	{
		printf("[%d] Type: %-12s Content: '%s' q_type: '%-12s'\n", i,
			get_token_type_name(token->type), token->content,
			get_token_quote_name(token->q_type));
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
		return ;
	}
	printf("\n=== ENVIRONMENT ===\n");
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	printf("===================\n\n");
}