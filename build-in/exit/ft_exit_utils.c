/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:30:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/05 14:24:51 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_overflow(char *str, int sign, int start_pos)
{
	long long	result;
	int			i;

	result = 0;
	i = start_pos;
	while (str[i] && ft_isdigit(str[i]))
	{
		if (result > (LLONG_MAX - (str[i] - '0')) / 10)
			return (1);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	result *= sign;
	if (result > INT_MAX || result < INT_MIN)
		return (1);
	return (0);
}

int	is_numeric(char *str)
{
	int	i;
	int	sign;
	int	digit_start;

	i = 0;
	sign = 1;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	digit_start = i;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || 
		str[i] == '\r' || str[i] == '\f' || str[i] == '\v'))
		i++;
	if (str[i] != '\0')
		return (0);
	return (!check_overflow(str, sign, digit_start));
}
