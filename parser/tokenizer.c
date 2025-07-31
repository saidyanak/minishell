/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/31 16:41:35 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	tokenize_input(char *input, t_base *base)
{
	int		i;
	t_token	*head;

	i = 0;
	head = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			handle_operator(input, &i, &head);
		else
		{
			handle_word_or_error(input, &i, &head);
			if (!head)
				break ;
		}
	}
	base->token = head;
}
