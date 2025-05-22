/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:38:34 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/22 20:17:43 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

void	add_token(t_token **head, char *str, t_token_type type)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	new->content = str;
	new->type = type;
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

char	*parse_word_with_quotes(char *input, int *i)
{
	int		start;
	char	*result;
	char	quote;
	char	*tmp;

	result = ft_strdup("");
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
			(*i)++;
			start = *i;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (!input[*i])
			{
				free(result);
				return (NULL); // ❌ tırnak kapanmadı
			}
			tmp = ft_substr(input, start, *i - start);
			result = ft_strjoin_free(result, tmp);
			(*i)++; // tırnağı geç
		}
		else
		{
			start = *i;
			while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
				&& input[*i] != '<' && input[*i] != '>' && input[*i] != '"'
				&& input[*i] != '\'')
				(*i)++;
			tmp = ft_substr(input, start, *i - start);
			result = ft_strjoin_free(result, tmp);
		}
	}
	return (result);
}

void	tokenize_input(char *input, t_base *base)
{
	int		i;
	char	*word;
	t_token	*head;

	i = 0;
	head = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|')
		{
			add_token(&head, ft_strdup("|"), TOKEN_PIPE);
			i++;
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			// yönlendirmeleri handle edebilirsin buradan
			i++; // basitçe geçelim
		}
		else
		{
			word = parse_word_with_quotes(input, &i);
			if (!word)
			{
				printf("minishell: syntax error: unclosed quote\n");
				// TODO: token freele
				return ;
			}
			add_token(&head, word, TOKEN_WORD);
		}
	}
	base->token = head;
}
