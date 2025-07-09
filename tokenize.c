/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:30:21 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/09 16:03:12 by yuocak           ###   ########.fr       */
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

static char	*create_temp_string(t_gc *gc, const char *str)
{
	char	*new_str;
	int		len;
	int		i;

	len = ft_strlen(str);
	new_str = ft_malloc_temp(gc, len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
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
void	add_token(t_token **head, char *str, t_token_type type, t_gc *gc)
{
	t_token	*new;
	t_token	*tmp;

	if (!str)
		return;
	
	new = ft_malloc_temp(gc, sizeof(t_token));
	if (!new)
		return;
	new->content = str;
	new->type = type;
	new->next = NULL;
	{
		free(str);
		return;
	}
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

static void	handle_word_or_error(char *input, int *i, t_token **head, t_gc *gc)
{
	char			*word;
	t_token_type	type;

	word = parse_word_with_quotes(input, i, &type);
	if (!word)
	{
		printf("minishell: syntax error: unclosed quote\n");
		// Token listesini temizle ve çık
		free_tokens(*head);
		*head = NULL;
		return;
	}
	add_token(head, word, type, gc);
}

static void	handle_operator(char *input, int *i, t_token **head, t_gc *gc)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(head, create_temp_string(gc, "<<"), TOKEN_HEREDOC, gc);
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(head, create_temp_string(gc, ">>"), TOKEN_APPEND, gc);
		(*i) += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(head, create_temp_string(gc, "|"), TOKEN_PIPE, gc);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(head, create_temp_string(gc, "<"), TOKEN_REDIRECT_IN, gc);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(head, create_temp_string(gc, ">"), TOKEN_REDIRECT_OUT, gc);
		(*i)++;
	}
}

void	tokenize_input(char *input, t_base *base)
{
	int		i;
	t_token	*head;

	if (!input || !base)
		return;
	
	i = 0;
	head = NULL;
	
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			handle_operator(input, &i, &head, &base->gc);
		else
		{
			handle_word_or_error(input, &i, &head, &base->gc);
			if (!head) // Hata durumunda çık
				break;
		}
	}
	base->token = head;
}


