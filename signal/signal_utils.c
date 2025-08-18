/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 18:42:04 by yuocak           ###   ########.fr       */
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
