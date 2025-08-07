/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:43:01 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_heredoc_info	*create_heredoc_node(int heredoc_id)
{
	t_heredoc_info	*node;

	node = malloc(sizeof(t_heredoc_info));
	if (!node)
		return (NULL);
	node->temp_filename = NULL;
	node->original_delimiter = NULL;
	node->heredoc_id = heredoc_id;
	node->next = NULL;
	return (node);
}

void	add_heredoc_to_list(t_heredoc_info **head, t_heredoc_info *new_node)
{
	t_heredoc_info	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_heredoc_info	*find_heredoc_by_id(t_heredoc_info *head, int id)
{
	t_heredoc_info	*current;

	current = head;
	while (current)
	{
		if (current->heredoc_id == id)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	cleanup_heredocs_list(t_heredoc_info *head)
{
	t_heredoc_info	*current;
	t_heredoc_info	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->temp_filename)
		{
			unlink(current->temp_filename);
			free(current->temp_filename);
		}
		if (current->original_delimiter)
			free(current->original_delimiter);
		free(current);
		current = next;
	}
}

void	cleanup_failed_heredocs_list(t_heredoc_info *head)
{
	cleanup_heredocs_list(head);
}