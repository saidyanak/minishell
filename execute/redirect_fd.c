/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:58:02 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/17 00:00:00 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

void	save_standard_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

void	restore_standard_fds(int saved_stdin, int saved_stdout)
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
}

static int	execute_with_redirection(t_base *base, t_token *cmd_tokens)
{
	t_token	*original_token;
	int		exit_status;

	if (!cmd_tokens)
		return (0);
	original_token = base->token;
	base->token = cmd_tokens;
	exit_status = single_execute_command(base);
	base->token = original_token;
	return (exit_status);
}

int	handle_redirection_execution(t_base *base)
{
	int		saved_stdin;
	int		saved_stdout;
	t_token	*cmd_tokens;
	int		exit_status;

	saved_stdin = -1;
	saved_stdout = -1;
	cmd_tokens = extract_command_tokens(base->token);
	save_standard_fds(&saved_stdin, &saved_stdout);
	if (handle_redirections(base->token, base) == -1)
	{
		restore_standard_fds(saved_stdin, saved_stdout);
		if (cmd_tokens)
			free_tokens(cmd_tokens);
		return (1);
	}
	exit_status = execute_with_redirection(base, cmd_tokens);
	if (cmd_tokens)
		free_tokens(cmd_tokens);
	restore_standard_fds(saved_stdin, saved_stdout);
	return (exit_status);
}