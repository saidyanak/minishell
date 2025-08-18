/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 19:52:58 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

int	check_redirection(t_token *token)
{
	t_token	*head;

	if (!token)
		return (0);
	head = token;
	while (head)
	{
		if (head->type == TOKEN_PIPE)
			return (0);
		if (head->type == TOKEN_APPEND || head->type == TOKEN_HEREDOC
			|| head->type == TOKEN_REDIRECT_IN
			|| head->type == TOKEN_REDIRECT_OUT)
			return (1);
		head = head->next;
	}
	return (0);
}

int	has_pipe_token(t_token *token)
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

int	single_execute_command(t_base *base)
{
	int	result;

	if (!base->token)
		return (1);
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(base->token->content))
		{
			ft_build_in(base->token, base);
			result = base->exit_status;
		}
		else
		{
			result = execute_external_command(base);
		}
		return (result);
	}
	return (0);
}

void	execute_command(t_base *base)
{
	if (!base->token)
		return ;
	if (!check_syntax_errors(base->token))
	{
		base->exit_status = 2;
		return ;
	}
	if (has_pipe_token(base->token))
	{
		base->exit_status = execute_multiple_command(base);
		base->token = NULL;
	}
	else if (check_redirection(base->token))
	{
		base->exit_status = handle_redirection_execution(base);
	}
	else
	{
		set_underscore_variable(base, base->token);
		base->exit_status = single_execute_command(base);
	}
}
