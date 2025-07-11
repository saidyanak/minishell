/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:03:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/10 18:21:06 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_list(t_gc_node **head)
{
    t_gc_node   *tmp;

    while (*head)
    {
        tmp = (*head)->next;
        free((*head)->ptr);
        free(*head);
        *head = tmp;
    }
}

void    ft_free_all_env(t_gc *gc)
{
    free_list(&gc->env_list);
}

void    ft_free_all_tmp(t_gc *gc)
{
    free_list(&gc->tmp_list);
}

void    ft_free_all(t_gc *gc)
{
    ft_free_all_env(gc);
    ft_free_all_tmp(gc);
}
