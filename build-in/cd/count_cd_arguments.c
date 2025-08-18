/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_cd_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:42:34 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 15:43:05 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_cd_arguments(t_token *token)
{
	t_token	*current;
	int		count;

	count = 0;
	current = token;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			count++;
		current = current->next;
	}
	return (count - 1);
}
