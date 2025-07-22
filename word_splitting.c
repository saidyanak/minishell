/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/07/22 10:12:59 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_ifs_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static void	skip_ifs_chars(char *str, int *i)
{
	while (str[*i] && is_ifs_char(str[*i]))
		(*i)++;
}

static char	*extract_word(char *str, int *i)
{
	int		start;
	char	*word;

	skip_ifs_chars(str, i);
	if (!str[*i])
		return (NULL);
	start = *i;
	while (str[*i] && !is_ifs_char(str[*i]))
		(*i)++;
	if (*i == start)
		return (NULL);
	word = ft_substr(str, start, *i - start);
	return (word);
}

static t_token	*create_word_token(char *content)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(content);
	token->type = TOKEN_WORD;
	token->q_type = NONE_QUOTE;
	token->next = NULL;
	return (token);
}

static void	add_token_to_list(t_token **head, t_token **tail,
		t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		*tail = new_token;
	}
}

static t_token	*split_string_to_tokens(char *str)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_token;
	char	*word;
	int		i;

	if (!str)
		return (NULL);
	head = NULL;
	tail = NULL;
	i = 0;
	while (str[i])
	{
		word = extract_word(str, &i);
		if (!word)
			break ;
		new_token = create_word_token(word);
		free(word);
		if (!new_token)
			break ;
		add_token_to_list(&head, &tail, new_token);
	}
	return (head);
}

static int	should_split_token(t_token *token)
{
	int	i;

	if (!token || !token->content)
		return (0);
	if (token->q_type == SINGLE_QUOTE || token->q_type == DOUBLE_QUOTE)
		return (0);
	i = 0;
	while (token->content[i])
	{
		if (is_ifs_char(token->content[i]))
			return (1);
		i++;
	}
	return (0);
}

static t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

static void	free_token_content(t_token *token)
{
	if (!token)
		return ;
	if (token->content)
		free(token->content);
	free(token);
}

static void	connect_split_tokens(t_token *split_tokens, t_token *next_token)
{
	t_token	*last_split;

	if (!split_tokens)
		return ;
	last_split = get_last_token(split_tokens);
	if (last_split)
		last_split->next = next_token;
}

static void	replace_token_with_split(t_token **head, t_token *current,
		t_token *prev)
{
	t_token	*split_tokens;
	t_token	*next_token;

	next_token = current->next;
	split_tokens = split_string_to_tokens(current->content);
	if (split_tokens)
	{
		connect_split_tokens(split_tokens, next_token);
		if (prev)
			prev->next = split_tokens;
		else
			*head = split_tokens;
	}
	else
	{
		if (prev)
			prev->next = next_token;
		else
			*head = next_token;
	}
	free_token_content(current);
}

static t_token	*process_current_token(t_token **head, t_token *current,
		t_token *prev)
{
	t_token	*next;

	next = current->next;
	if (should_split_token(current))
	{
		replace_token_with_split(head, current, prev);
		return (next);
	}
	return (current);
}

void	word_splitting(t_base *base)
{
	t_token *current;
	t_token *prev;
	t_token *result;

	if (!base || !base->token)
		return ;
	current = base->token;
	prev = NULL;
	while (current)
	{
		result = process_current_token(&base->token, current, prev);
		if (result == current)
		{
			prev = current;
			current = current->next;
		}
		else
		{
			if (prev && prev->next)
			{
				while (prev->next && prev->next != result)
					prev = prev->next;
			}
			current = result;
		}
	}
}