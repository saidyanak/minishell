/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 11:38:40 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	tokenize_input(char *input, t_base *base)
{
	int		i;
	t_token	*head;

	if (!input || !base)
		return ;
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
