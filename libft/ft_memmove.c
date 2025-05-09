/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:11:36 by yuocak            #+#    #+#             */
/*   Updated: 2025/04/09 13:21:58 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*t_src;
	char	*t_dst;

	if (!src && !dst)
		return (NULL);
	t_src = (char *)src;
	t_dst = (char *)dst;
	if (t_dst > t_src)
	{
		while (len-- > 0)
			t_dst[len] = t_src[len];
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}
