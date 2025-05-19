/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:38:34 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/19 12:41:03 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	quote_control(char *input)
{
	int	i;
	int	cont;

	i = 0;
	while (input[i])
	{
		i++;
	}
	return (0);
}

void	tokenize_input(char *input, t_base my_base)
{
	int	i;

	i = 0;
	if (quote_control(input))
		return ;
	while (input[i])
	{
		i++;
	}
}
