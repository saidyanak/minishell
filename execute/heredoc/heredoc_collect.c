/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:49:06 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	setup_heredoc_node(t_token *heredoc_token, t_heredoc_info *info_node)
{
	if (!heredoc_token->next || !heredoc_token->next->content)
		return (0);
	info_node->original_delimiter = ft_strdup(heredoc_token->next->content);
	if (!info_node->original_delimiter)
		return (0);
	info_node->temp_filename = create_temp_filename(info_node->heredoc_id);
	if (!info_node->temp_filename)
	{
		free(info_node->original_delimiter);
		info_node->original_delimiter = NULL;
		return (0);
	}
	return (1);
}

int	create_temp_file_from_content(t_heredoc_info *info_node, char *content)
{
	if (!write_content_to_temp_file(info_node->temp_filename, content))
	{
		free(content);
		unlink(info_node->temp_filename);
		free(info_node->temp_filename);
		free(info_node->original_delimiter);
		info_node->temp_filename = NULL;
		info_node->original_delimiter = NULL;
		return (0);
	}
	free(content);
	return (1);
}

int	create_placeholder_token(t_token *heredoc_token, t_heredoc_info *info_node)
{
	char	*placeholder;

	placeholder = create_heredoc_placeholder(info_node->heredoc_id);
	if (!placeholder)
	{
		unlink(info_node->temp_filename);
		free(info_node->temp_filename);
		free(info_node->original_delimiter);
		info_node->temp_filename = NULL;
		info_node->original_delimiter = NULL;
		return (0);
	}
	free(heredoc_token->next->content);
	heredoc_token->next->content = placeholder;
	return (1);
}

int	process_single_heredoc_child(t_token *heredoc_token,
		t_heredoc_info *info_node, t_base *base)
{
	char	*content;

	if (!setup_heredoc_node(heredoc_token, info_node))
		return (0);
	content = run_heredoc_child_to_file(heredoc_token->next->content, base,
			info_node);
	if (!content)
	{
		unlink(info_node->temp_filename);
		free(info_node->temp_filename);
		free(info_node->original_delimiter);
		info_node->temp_filename = NULL;
		info_node->original_delimiter = NULL;
		return (0);
	}
	if (!create_temp_file_from_content(info_node, content))
		return (0);
	return (create_placeholder_token(heredoc_token, info_node));
}

int	collect_heredocs_child(t_token *token, t_base *base)
{
	if (!create_heredoc_list(token, base))
		return (0);
	return (process_heredoc_list(token, base));
}
