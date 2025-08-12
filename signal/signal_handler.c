/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:01:15 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/12 08:50:03 by syanak           ###   ########.fr       */
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
	else if (control == -1)
	{
		flag = 0;
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
	// Global signal kontrolü
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
	// HEREDOC signal kontrolü ekle
	if (*heredoc_static_flag(0) == 1)
	{
		heredoc_static_flag(-1); // Reset et
		return (130);
	}
	return (exit_status);
}
void	set_g_signal(int sig)
{
	g_signal = sig;
}

void	restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
