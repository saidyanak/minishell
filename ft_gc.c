/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:03:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/09 15:49:41 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_init(t_gc *gc)
{
	gc->permanent = NULL;
	gc->temporary = NULL;
}

static t_mem_node	*create_node(void *ptr)
{
	t_mem_node	*node;

	node = malloc(sizeof(t_mem_node));
	if (!node)
		return (NULL);
	node->ptr = ptr;
	node->next = NULL;
	return (node);
}

static void	*add_to_list(t_mem_node **head, size_t size)
{
	void		*ptr;
	t_mem_node	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = create_node(ptr);
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->next = *head;
	*head = node;
	return (ptr);
}

void	*ft_malloc_perm(t_gc *gc, size_t size)
{
	return (add_to_list(&gc->permanent, size));
}

void	*ft_malloc_temp(t_gc *gc, size_t size)
{
	return (add_to_list(&gc->temporary, size));
}

static void	free_all_from_list(t_mem_node **head)
{
	t_mem_node	*current;
	t_mem_node	*next;

	current = *head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	*head = NULL;
}

void	ft_free_all_temp(t_gc *gc)
{
	free_all_from_list(&gc->temporary);
}

void	ft_free_all_perm(t_gc *gc)
{
	free_all_from_list(&gc->permanent);
}

void	ft_free_all(t_gc *gc)
{
	ft_free_all_temp(gc);
	ft_free_all_perm(gc);
}