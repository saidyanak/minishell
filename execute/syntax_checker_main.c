/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/13 20:05:53 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}

int	is_operator_token(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

static int	check_consecutive_operators(t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = token;
	prev = NULL;
	while (current)
	{
		if (is_operator_token(current->type) && prev
			&& is_operator_token(prev->type))
		{
			if (current->type == prev->type)
			{
				print_syntax_error(current->content);
				return (0);
			}
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

static int	check_ending_operator(t_token *token)
{
	t_token	*current;
	t_token	*last;

	if (!token)
		return (1);
	current = token;
	last = NULL;
	while (current)
	{
		last = current;
		current = current->next;
	}
	if (last && is_redirection_token(last->type))
	{
		print_syntax_error("newline");
		return (0);
	}
	return (1);
}

int	check_syntax_errors(t_token *token)
{
	if (!token)
		return (1);
	if (!check_pipe_syntax(token))
		return (0);
	if (!check_redirection_syntax(token))
		return (0);
	if (!check_heredoc_syntax(token))
		return (0);
	if (!check_consecutive_operators(token))
		return (0);
	if (!check_ending_operator(token))
		return (0);
	return (1);
}
