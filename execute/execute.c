/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 14:50:46 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	single_execute_command(t_base *base)
{
	if (!base->token)
		return (1);
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(base->token->content))
		{
			ft_build_in(base->token, base);
			return (base->exit_status);
		}
		else
		{
			return (execute_external_command(base));
		}
	}
	return (0);
}

void	execute_command(t_base *base)
{
	if (!base->token)
		return ;
	if (!check_syntax_errors(base->token))
	{
		base->exit_status = 2;
		return ;
	}
	if (has_special_tokens(base->token))
	{
		base->exit_status = execute_multiple_command(base);
		base->token = NULL;
	}
	else
	{
		base->exit_status = single_execute_command(base);
	}
	set_underscore_variable(base, base->token);
}
