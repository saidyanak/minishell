/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 17:43:43 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	has_input_redirection(t_token *cmd)
{
	while (cmd)
	{
		if (cmd->type == TOKEN_REDIRECT_IN || cmd->type == TOKEN_HEREDOC)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

int	has_output_redirection(t_token *cmd)
{
	while (cmd)
	{
		if (cmd->type == TOKEN_REDIRECT_OUT || cmd->type == TOKEN_APPEND)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
