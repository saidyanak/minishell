/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/16 15:58:36 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

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

static void	init_variable(t_token **current, t_token **prev, t_base *base)
{
	*current = base->token;
	*prev = NULL;
}

void	word_splitting(t_base *base)
{
	t_token	*current;
	t_token	*prev;
	t_token	*result;

	if (!base || !base->token)
		return ;
	init_variable(&current, &prev, base);
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
