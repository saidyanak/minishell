/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/22 09:39:15 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	is_special_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC || type == TOKEN_PIPE);
}

int	has_special_tokens(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (is_special_token(current->type))
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_pipe(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

t_token	*find_original_segment(t_token *token, int segment_index)
{
	t_token	*current;
	int		current_segment;

	current = token;
	current_segment = 0;
	while (current && current_segment < segment_index)
	{
		if (current->type == TOKEN_PIPE)
			current_segment++;
		current = current->next;
	}
	return (current);
}

void	free_segments(t_token **segments, int count)
{
	int	i;

	if (!segments)
		return ;
	i = 0;
	while (i < count)
	{
		if (segments[i])
			free_tokens(segments[i]);
		i++;
	}
	free(segments);
}

int	execute_copy_prompt(t_token *prompt_segment, t_base *base)
{
	t_token *original_token;
	int result;

	if (!prompt_segment)
		return (1);
	original_token = base->token;
	base->token = prompt_segment;
	result = single_execute_command(prompt_segment, base);
	base->token = original_token;
	return (result);
}