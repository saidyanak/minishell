/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:01:15 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/12 04:44:11 by syanak           ###   ########.fr       */
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
	// Force readline to exit immediately
	close(STDIN_FILENO);
	rl_done = 1;
	// Clear the current line buffer
	// Send a character to stdin to wake up readline
	// This prevents readline from waiting for another character
	// Set the heredoc flag
	heredoc_static_flag(1);
}

void	sigquit_handler(int sig)
{
	(void)sig;
	g_signal = SIGQUIT;
}

int	*heredoc_static_flag(int control)
{
	static int	flag = 0;

	if (control == 0)
		return (&flag);
	else if (control == 1)
	{
		flag = 1;
		return (&flag);
	}
	return (&flag);
}

void	sigint_execution_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
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
	return (exit_status);
}

void	restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
