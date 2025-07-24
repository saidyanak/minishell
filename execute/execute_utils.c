/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/24 13:32:08 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	is_special_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC || type == TOKEN_PIPE);
}

int	has_special_tokens(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (is_special_token(current->type))
			return (1);
		current = current->next;
	}
	return (0);
}
