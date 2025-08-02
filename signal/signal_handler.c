/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:01:15 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/02 13:45:29 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include   <signal.h>

volatile sig_atomic_t g_signal = 0;

void    sigint_handler(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    sigquit_handler(int sig)
{
    (void)sig;
    g_signal = SIGQUIT;
}

void    setup_interactive_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

void    setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void    setup_heredoc_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
}

void    setup_execution_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void    sigint_execution_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
}

int     check_signal_status(void)
{
    if (g_signal == SIGINT)
    {
        g_signal = 0;
        return (130); // Ctrl+C exit status
    }
    if (g_signal == SIGQUIT)
    {
        g_signal = 0;
        return (131); // Ctrl+\ exit status
    }
    return (0);
}

void    restore_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
