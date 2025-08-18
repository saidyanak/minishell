/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:01:15 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 18:46:15 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int		g_signal = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	close(STDIN_FILENO);
	rl_done = 1;
	heredoc_static_flag(1);
}

int	check_signal_status(int exit_status)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (130);
	}
	if (g_signal == SIGQUIT)
	{
		g_signal = 0;
		return (131);
	}
	if (*heredoc_static_flag(0) == 1)
	{
		g_signal = 0;
		heredoc_static_flag(-1);
		return (130);
	}
	return (exit_status);
}
