/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:24:36 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/10 17:45:13 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    gc_init(t_gc *gc)
{
    gc->env_list = NULL;
    gc->tmp_list = NULL;
}
void    *add_to_list(t_gc_node **head, int size)
{
    void        *ptr;
    t_gc_node   *node;

    ptr = malloc(size);
    if (!ptr)
        return (NULL);
    node = malloc(sizeof(t_gc_node));
    if (!node)
    {
        free(ptr);
        return (NULL);
    }
    node->ptr = ptr;
    node->next = *head;
    *head = node;
    return (ptr);
}

void    *ft_malloc_env(t_gc *gc, int size)
{
    return (add_to_list(&gc->env_list, size));
}

void    *ft_malloc_tmp(t_gc *gc, int size)
{
    return (add_to_list(&gc->tmp_list, size));
}