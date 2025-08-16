/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 16:00:22 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

void	handle_operator(char *input, int *i, t_token **head)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(head, ft_strdup("<<"), TOKEN_HEREDOC, NONE_QUOTE);
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(head, ft_strdup(">>"), TOKEN_APPEND, NONE_QUOTE);
		(*i) += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(head, ft_strdup("|"), TOKEN_PIPE, NONE_QUOTE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(head, ft_strdup("<"), TOKEN_REDIRECT_IN, NONE_QUOTE);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(head, ft_strdup(">"), TOKEN_REDIRECT_OUT, NONE_QUOTE);
		(*i)++;
	}
}
