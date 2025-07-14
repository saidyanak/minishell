/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:55:20 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/14 12:13:16 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strdup_gc(t_gc *gc, const char *str)
{
    char    *dup;
    int     len;
    int     i;

    i = 0;
    if (!str)
        return (NULL);
    len = ft_strlen(str);
    dup = ft_malloc_tmp(gc, len + 1);
    if (!dup)
        return (NULL);
    while (str[i])
    {
        dup[i] = str[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

char    *ft_substr_gc(t_gc *gc, char const *s, unsigned int start, size_t len)
{
    char    *substr;
    size_t  i;
    size_t  s_len;

    i = 0;
    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup_gc(gc, ""));
    if (len > s_len - start)
        len = s_len - start;
    substr = ft_malloc_tmp(gc, len + 1);
    if (!substr)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        substr[i] = s[start + i];
        i++;
    }
    substr[i] = '\0';
    return (substr);
}
