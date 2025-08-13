/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/14 00:32:02 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	single_execute_command(t_base *base)
{
	int	result;
	// int	saved_stdin;
	// int	saved_stdout;

	// if (check_redirection(base->token))
	// {
	// 	saved_stdin = dup(STDIN_FILENO);
	// 	saved_stdout = dup(STDOUT_FILENO);
	// 	swap_token(base->token);
	// 	handle_redirections(base->token, base);
	// 	single_execute_command(base);
	// 	dup2(saved_stdin, STDIN_FILENO);
	// 	close(saved_stdin);
	// 	dup2(saved_stdout, STDOUT_FILENO);
	// 	close(saved_stdout);
	// }
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
void	swap_token_data(t_token *a, t_token *b)
{
	char			*temp_content;
	t_token_type	temp_type;
	t_quote_type	temp_quote;

	temp_content = a->content;
	a->content = b->content;
	b->content = temp_content;
	temp_type = a->type;
	a->type = b->type;
	b->type = temp_type;
	temp_quote = a->q_type;
	a->q_type = b->q_type;
	b->q_type = temp_quote;
}

void	swap_token(t_token *token)
{
	t_token	*current;

	if (!token)
		return ;
	current = token;
	while (current && current->next && current->next->next)
	{
		// Pattern: [REDIRECT] [FILE] [COMMAND] -> [COMMAND] [REDIRECT] [FILE]
		if ((current->type == TOKEN_REDIRECT_OUT
				|| current->type == TOKEN_REDIRECT_IN
				|| current->type == TOKEN_APPEND
				|| current->type == TOKEN_HEREDOC)
			&& (current->next->next->type == TOKEN_WORD
				|| current->next->next->type == TOKEN_QUOTED_WORD))
		{
			swap_token_data(current, current->next->next);
			swap_token_data(current->next, current->next->next);
			return ;
		}
		current = current->next;
	}
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
	else if (has_special_tokens(base->token))
	{
		base->exit_status = execute_multiple_command(base);
		// Multiple command'da cleanup yapılıyor, token NULL yapılıyor
		base->token = NULL;
	}
	else
	{
		base->exit_status = single_execute_command(base);
		set_underscore_variable(base, base->token);
		// Single command için cleanup burada değil main.c'de yapılıyor
	}
}
