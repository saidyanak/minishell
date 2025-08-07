/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:43:03 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	create_heredoc_list(t_token *token, t_base *base)
{
	t_token			*current;
	t_heredoc_info	*new_node;
	int				heredoc_index;

	current = token;
	heredoc_index = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			new_node = create_heredoc_node(heredoc_index);
			if (!new_node)
			{
				cleanup_heredocs_list(base->heredocs);
				base->heredocs = NULL;
				return (0);
			}
			add_heredoc_to_list(&base->heredocs, new_node);
			heredoc_index++;
		}
		current = current->next;
	}
	return (1);
}

int	process_heredoc_list(t_token *token, t_base *base)
{
	t_token			*current;
	t_heredoc_info	*node;
	int				heredoc_index;

	current = token;
	heredoc_index = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			node = find_heredoc_by_id(base->heredocs, heredoc_index);
			if (!node || !process_single_heredoc_child(current, node, base))
			{
				cleanup_heredocs_list(base->heredocs);
				base->heredocs = NULL;
				return (0);
			}
			heredoc_index++;
		}
		current = current->next;
	}
	return (1);
}

int	is_heredoc_placeholder(char *str)
{
	if (!str)
		return (0);
	return (ft_strncmp(str, "__HEREDOC_", 10) == 0);
}

int	extract_heredoc_id(char *placeholder)
{
	char	*id_str;
	int		id;

	if (!placeholder || ft_strlen(placeholder) <= 10)
		return (-1);
	id_str = placeholder + 10;
	id = ft_atoi(id_str);
	return (id);
}

void	cleanup_heredocs(t_base *base)
{
	if (!base)
		return ;
	if (base->heredocs)
	{
		cleanup_heredocs_list(base->heredocs);
		base->heredocs = NULL;
	}
	base->heredoc_count = 0;
}