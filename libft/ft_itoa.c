/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:05:09 by yuocak            #+#    #+#             */
/*   Updated: 2024/11/03 17:55:48 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	len_count(int n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*str;
	int			len;
	long int	num;

	num = n;
	len = len_count(num);
	if (num == 0)
		return (ft_strdup("0"));
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (0 > num)
	{
		str[0] = '-';
		num = -num;
	}
	str[len] = '\0';
	while (num > 0)
	{
		str[--len] = (num % 10) + '0';
		num = num / 10;
	}
	return (str);
}
