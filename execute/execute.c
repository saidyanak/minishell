/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/07 15:45:25 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	single_execute_command(t_base *base)
{
	int	result;

	if (!base->token)
		return (1);
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(base->token->content))
		{
			ft_build_in(base->token, base);
			result = base->exit_status;
		}
		else
		{
			result = execute_external_command(base);
		}
		return (result);
	}
	return (0);
}

void	execute_command(t_base *base)
{
	if (!base->token)
	{
		// Token yoksa bile heredoc'ları temizle
		cleanup_heredocs(base);
		return ;
	}
	if (!check_syntax_errors(base->token))
	{
		base->exit_status = 2;
		// Syntax error durumunda heredoc'ları temizle
		cleanup_heredocs(base);
		return ;
	}
	if (has_special_tokens(base->token))
	{
		base->exit_status = execute_multiple_command(base);
		// Multiple command'da cleanup yapılıyor, token NULL yapılıyor
		base->token = NULL;
	}
	else
	{
		base->exit_status = single_execute_command(base);
		// Single command için cleanup burada değil main.c'de yapılıyor
	}
}