/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting_utils_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:48:04 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 14:49:27 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_ifs_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	skip_ifs_chars(char *str, int *i)
{
	while (str[*i] && is_ifs_char(str[*i]))
		(*i)++;
}
