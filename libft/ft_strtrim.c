/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:18:06 by yuocak            #+#    #+#             */
/*   Updated: 2025/04/09 13:21:09 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(const char *s1, const char *set)
{
	char		*finish;
	char		*start;
	size_t		len;

	finish = (char *)s1 + ft_strlen(s1) - 1;
	while (*s1 && ft_strchr(set,*s1))
		s1++;
	while (finish > s1 && ft_strchr(set,*finish))
		finish--;
	len = finish - s1 + 1;
	start = (char *)malloc(sizeof(char) * len + 1);
	if (!start)
		return (NULL);
	ft_strlcpy(start, s1, len + 1);
	return (start);
}
