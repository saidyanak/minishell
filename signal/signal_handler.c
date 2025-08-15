/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:01:15 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 16:58:49 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	sigquit_handler(int sig)
{
	(void)sig;
	g_signal = SIGQUIT;
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
		heredoc_static_flag(-1);
		return (130);
	}
	return (exit_status);
}

void	set_g_signal(int sig)
{
	g_signal = sig;
}
