/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:21:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/05 16:21:41 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

// ft_strchr: '=' karakterini bulur
char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

// ft_strdup: string’i kopyalar
char	*ft_strdup(const char *s1)
{
	int		len;
	char	*copy;
    int     i;

    i = 0;
	len = 0;
	while (s1[len])
		len++;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	while (i < len)
    {
		copy[i] = s1[i];
        i++;
    }
	copy[len] = '\0';
	return (copy);
}

// strndup: sadece belirli uzunluk kadar kopyalar
char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;

	copy = malloc(n + 1);
	if (!copy)
		return (NULL);
	for (size_t i = 0; i < n; i++)
		copy[i] = s1[i];
	copy[n] = '\0';
	return (copy);
}
