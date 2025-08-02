/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singal_handler_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:44:36 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/02 14:45:15 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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