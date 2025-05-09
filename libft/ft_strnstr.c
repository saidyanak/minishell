/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:01:24 by yuocak            #+#    #+#             */
/*   Updated: 2025/04/09 13:21:02 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *neddle, size_t len)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if (neddle[0] == '\0')
	{
		return ((char *)haystack);
	}
	while (haystack[i] && i < len)
	{
		while (haystack[i + j] == neddle[j] && haystack[i + j] && i + j < len)
		{
			j++;
			if (neddle[j] == '\0')
			{
				return ((char *)haystack + i);
			}
		}
		i++;
		j = 0;
	}
	return (NULL);
}
