/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:30:21 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/08 16:05:55 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || ft_isspace(c) || c == '\0');
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

void	add_token(t_token **head, char *str, t_token_type type,
		t_quote_type q_type)
{
	t_token	*new;
	t_token	*tmp;

	if (!str)
		return ;
	new = malloc(sizeof(t_token));
	if (!new)
	{
		free(str);
		return ;
	}
	new->content = str;
	new->type = type;
	new->q_type = q_type;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	handle_word_or_error(char *input, int *i, t_token **head)
{
	char			*word;
	t_token_type	type;
	t_quote_type	q_type;

	word = parse_word_with_quotes(input, i, &type, &q_type);
	if (!word)
	{
		printf("minishell: syntax error: unclosed quote\n");
		free_tokens(*head);
		*head = NULL;
		return ;
	}
	add_token(head, word, type, q_type);
}

static void	handle_operator(char *input, int *i, t_token **head)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(head, ft_strdup("<<"), TOKEN_HEREDOC, NONE_QUOTE);
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(head, ft_strdup(">>"), TOKEN_APPEND, NONE_QUOTE);
		(*i) += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(head, ft_strdup("|"), TOKEN_PIPE, NONE_QUOTE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(head, ft_strdup("<"), TOKEN_REDIRECT_IN, NONE_QUOTE);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(head, ft_strdup(">"), TOKEN_REDIRECT_OUT, NONE_QUOTE);
		(*i)++;
	}
}

void	tokenize_input(char *input, t_base *base)
{
	int i;
	t_token *head;

	if (!input || !base)
		return ;
	i = 0;
	head = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			handle_operator(input, &i, &head);
		else
		{
			handle_word_or_error(input, &i, &head);
			if (!head)
				break ;
		}
	}
	base->token = head;
}