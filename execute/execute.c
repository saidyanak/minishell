/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 15:54:31 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redirection(t_token *token)
{
	t_token	*head;
	int		type;

	type = 0;
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
			type = 1;
		head = head->next;
	}
	return (type);
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
	int saved_stdin;
	int saved_stdout;

	if (!base->token)
		return ;
	saved_stdin = -1;
	saved_stdout = -1;
	if (!check_syntax_errors(base->token))
	{
		base->exit_status = 2;
		return ;
	}
	if (check_redirection(base->token))
	{
		if (base->token->type != TOKEN_WORD
			&& base->token->type != TOKEN_QUOTED_WORD)
			swap_token(base->token);
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (handle_redirections(base->token, base) == -1)
		{
			if (saved_stdin != -1)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
			if (saved_stdout != -1)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
			}
			base->exit_status = 1;
			return ;
		}
		base->exit_status = single_execute_command(base);
		if (saved_stdin != -1)
		{
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
		}
		if (saved_stdout != -1)
		{
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
		}
		return ;
	}
	if (has_special_tokens(base->token))
	{
		base->exit_status = execute_multiple_command(base);
		base->token = NULL;
	}
	else
	{
		base->exit_status = single_execute_command(base);
		set_underscore_variable(base, base->token);
	}
}