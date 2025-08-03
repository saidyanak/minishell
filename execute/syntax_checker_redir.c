/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/03 13:28:53 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_redirection_file(t_token *current)
{
	if (!current->next)
	{
		print_syntax_error("newline");
		return (0);
	}
	if (is_redirection_token(current->next->type))
	{
		print_syntax_error(current->next->content);
		return (0);
	}
	if (current->next->type != TOKEN_WORD
		&& current->next->type != TOKEN_QUOTED_WORD)
	{
		print_syntax_error(current->next->content);
		return (0);
	}
	return (1);
}

int	check_redirection_syntax(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (is_redirection_token(current->type))
		{
			if (!validate_redirection_file(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_heredoc_syntax(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			if (current->next->type != TOKEN_WORD
				&& current->next->type != TOKEN_QUOTED_WORD)
			{
				print_syntax_error(current->next->content);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
