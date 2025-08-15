/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:58:02 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 19:08:20 by yuocak           ###   ########.fr       */
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

int	handle_redirection_execution(t_base *base)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (base->token->type != TOKEN_WORD
		&& base->token->type != TOKEN_QUOTED_WORD)
		swap_token(base->token);
	save_standard_fds(&saved_stdin, &saved_stdout);
	if (handle_redirections(base->token, base) == -1)
	{
		restore_standard_fds(saved_stdin, saved_stdout);
		return (1);
	}
	base->exit_status = single_execute_command(base);
	restore_standard_fds(saved_stdin, saved_stdout);
	return (base->exit_status);
}
