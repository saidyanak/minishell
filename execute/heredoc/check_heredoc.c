/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:13:20 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 20:21:46 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	handle_heredoc_processing(t_base *base)
{
	heredoc_static_flag(-1);
	base->exit_status = 0;
	process_all_heredocs(base);
	if (*heredoc_static_flag(0) == 1 || base->exit_status == 130)
	{
		base->exit_status = 130;
		cleanup_tokens_and_heredocs(base);
		return (0);
	}
	if (base->exit_status != 0)
	{
		cleanup_tokens_and_heredocs(base);
		return (0);
	}
	return (1);
}

int	check_heredoc(t_token *token)
{
	t_token	*head;

	head = token;
	while (head)
	{
		if (head->type == TOKEN_HEREDOC)
		{
			return (1);
		}
		head = head->next;
	}
	return (0);
}