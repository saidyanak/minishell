/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 19:07:11 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

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
