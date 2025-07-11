/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:30:21 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/11 15:55:05 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

char	*ft_strdup_gc(t_gc *gc, const char *str)
{
	char	*dup;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	dup = ft_malloc_tmp(gc, len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_substr_gc(t_gc *gc, char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup_gc(gc, ""));
	if (len > s_len - start)
		len = s_len - start;
	substr = ft_malloc_tmp(gc, len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || ft_isspace(c) || c == '\0');
}
char	*ft_strjoin_free_gc(char *s1, char *s2, t_gc *gc)
{
	char	*temp;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	temp = ft_strjoin(s1, s2);
	res = ft_strdup_gc(gc, temp);
	free(temp);
	return (res);
}
void	add_token(t_token **head, char *str, t_token_type type, t_gc *gc)
{
	t_token	*new;
	t_token	*tmp;

	if (!str)
		return;
	
	new = ft_malloc_tmp(gc, sizeof(t_token));
	if (!new)
	{
		return;
	}
	new->content = str;
	new->type = type;
	new->q_type = NONE_QUOTE; // Initialize quote type to avoid uninitialized value
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

	word = parse_word_with_quotes(input, i, &type, gc);
	if (!word)
	{
		printf("minishell: syntax error: unclosed quote\n");
		// Token listesi GC tarafından otomatik temizlenecek
		*head = NULL;
		return;
	}
	add_token(head, word, type, gc);
}

static void	handle_operator(char *input, int *i, t_token **head, t_gc *gc)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(head, ft_strdup("<<"), TOKEN_HEREDOC, gc);
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(head, ft_strdup_gc(gc, ">>"), TOKEN_APPEND, gc);
		(*i) += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(head, ft_strdup_gc(gc, "|"), TOKEN_PIPE, gc);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(head, ft_strdup_gc(gc, "<"), TOKEN_REDIRECT_IN, gc);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(head, ft_strdup_gc(gc, ">"), TOKEN_REDIRECT_OUT, gc);
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
			handle_operator(input, &i, &head, base->gc);
		else
		{
			handle_word_or_error(input, &i, &head, base->gc);
			if (!head) // Hata durumunda çık
				break;
		}
	}
	base->token = head;
}


