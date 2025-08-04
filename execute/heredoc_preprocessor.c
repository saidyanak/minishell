/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_preprocessor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/07/31 13:00:00 by syanak            #+#    #+#             */
/*   Updated: 2025/07/31 13:00:00 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>


static int	count_heredocs(t_token *token)
{
	t_token *current;
	int count;

	count = 0;
	current = token;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			count++;
		current = current->next;
	}
	return (count);
}

static char	*create_heredoc_placeholder(int heredoc_id)
{
	char *id_str;
	char *placeholder;

	id_str = ft_itoa(heredoc_id);
	if (!id_str)
		return (NULL);
	placeholder = ft_strjoin("__HEREDOC_", id_str);
	free(id_str);
	return (placeholder);
}

static t_heredoc_info	*create_heredoc_info_array(int count)
{
	t_heredoc_info *array;
	int i;

	array = malloc(sizeof(t_heredoc_info) * count);
	if (!array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		array[i].content = NULL;
		array[i].original_delimiter = NULL;
		array[i].heredoc_id = i;
		i++;
	}
	return (array);
}

static void	cleanup_failed_heredocs(t_heredoc_info *heredocs, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		if (heredocs[i].content)
			free(heredocs[i].content);
		if (heredocs[i].original_delimiter)
			free(heredocs[i].original_delimiter);
		i++;
	}
}

static void	cleanup_heredoc_info(t_heredoc_info *info)
{
	if (info->original_delimiter)
	{
		free(info->original_delimiter);
		info->original_delimiter = NULL;
	}
	if (info->content)
	{
		free(info->content);
		info->content = NULL;
	}
}

static int	process_single_heredoc(t_token *heredoc_token, t_heredoc_info *info,
		t_base *base)
{
	char *placeholder;

	if (!heredoc_token->next || !heredoc_token->next->content)
		return (0);
	info->original_delimiter = ft_strdup(heredoc_token->next->content);
	if (!info->original_delimiter)
		return (0);
	info->content = handle_heredoc(heredoc_token->next->content, base);
	if (!info->content)
	{
		cleanup_heredoc_info(info);
		return (0);
	}
	placeholder = create_heredoc_placeholder(info->heredoc_id);
	if (!placeholder)
	{
		cleanup_heredoc_info(info);
		return (0);
	}
	free(heredoc_token->next->content);
	heredoc_token->next->content = placeholder;
	return (1);
}

static int	collect_heredocs(t_token *token, t_heredoc_info *heredocs,
		t_base *base)
{
	t_token *current;
	int heredoc_index;

	current = token;
	heredoc_index = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (!process_single_heredoc(current, &heredocs[heredoc_index],
					base))
			{
				cleanup_failed_heredocs(heredocs, heredoc_index);
				return (0);
			}
			heredoc_index++;
		}
		current = current->next;
	}
	return (1);
}

static void	store_heredocs_in_base(t_base *base, t_heredoc_info *heredocs,
		int count)
{
	base->heredoc_count = count;
	base->heredocs = heredocs;
}

int	preprocess_heredocs(t_base *base)
{
	int heredoc_count;
	t_heredoc_info *heredocs;

	if (!base || !base->token)
		return (1);
	heredoc_count = count_heredocs(base->token);
	if (heredoc_count == 0)
		return (1);
	heredocs = create_heredoc_info_array(heredoc_count);
	if (!heredocs)
		return (0);
	if (!collect_heredocs(base->token, heredocs, base))
	{
		free(heredocs);
		return (0);
	}
	store_heredocs_in_base(base, heredocs, heredoc_count);
	return (1);
}