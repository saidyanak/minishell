/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/07 06:56:30 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	multiple_execute_command(t_token *token, t_base *base)
{
	(void)token;
	(void)base;
	printf("Multiple command execution not implemented yet\n");
}

int	single_execute_command(t_token *token, t_base *base)
{
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(token->content))
			ft_build_in(token, base);
		else
			return (execute_external_command(token, base));
	}
	return (0);
}

void	execute_command(t_base *base)
{
	t_token	*current_promt;

	current_promt = base->token;
	if (is_single_execute(current_promt))
		single_execute_command(current_promt, base);
	else
		multiple_execute_command(current_promt, base);
}
