/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:30:21 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/19 15:02:16 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
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

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	res = ft_strjoin(s1, s2);
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

static void	handle_word_or_error(char *input, int *i, t_token **head)
{
	char	*word;

	word = parse_word_with_quotes(input, i);
	if (!word)
	{
		printf("minishell: syntax error: unclosed quote\n");
		//freleme yap;
		return ;
	}
	add_token(head, word, TOKEN_WORD);
}

static void	handle_opeartor(char *input, int *i, t_token **head)
{
	if (input[*i] == '|')
	{
		add_token(head, ft_strdup("|"), TOKEN_PIPE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(head, ft_strdup("<"), TOKEN_REDIRECT_IN);
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(head, ft_strdup("<<"), TOKEN_HEREDOC);
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		add_token(head, ft_strdup(">"), TOKEN_REDIRECT_OUT);
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(head, ft_strdup(">>"), TOKEN_APPEND);
		(*i) += 2;
	}
}

void	tokenize_input(char *input, t_base *base)
{
	int		i;
	t_token	*head;

	i = 0;
	head = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			handle_opeartor(input, &i, &head);
		else
			handle_word_or_error(input, &i, &head);
	}
	base->token = head;
}


