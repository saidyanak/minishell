/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 16:00:48 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

static int	check_pipe_start(t_token *current, t_token *prev)
{
	(void)current;
	if (!prev)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

static int	check_pipe_end(t_token *current)
{
	if (!current->next)
	{
		print_syntax_error("newline");
		return (0);
	}
	if (current->next->type == TOKEN_PIPE)
	{
		print_syntax_error(current->next->content);
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *current, t_token *prev)
{
	(void)current;
	if (prev && prev->type == TOKEN_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

int	check_pipe_syntax(t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = token;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!check_pipe_start(current, prev))
				return (0);
			if (!check_pipe_end(current))
				return (0);
			if (!check_consecutive_pipes(current, prev))
				return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
