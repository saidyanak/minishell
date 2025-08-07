/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:43:07 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <fcntl.h>

char	*create_temp_filename(int heredoc_id)
{
	char	*id_str;
	char	*temp_name;
	char	*full_path;

	id_str = ft_itoa(heredoc_id);
	if (!id_str)
		return (NULL);
	temp_name = ft_strjoin("minishell_heredoc_", id_str);
	free(id_str);
	if (!temp_name)
		return (NULL);
	full_path = ft_strjoin("/tmp/", temp_name);
	free(temp_name);
	return (full_path);
}

int	write_content_to_temp_file(char *filename, char *content)
{
	int		fd;
	ssize_t	written;
	size_t	len;

	if (!content)
		return (1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("open temp file");
		return (0);
	}
	len = ft_strlen(content);
	written = write(fd, content, len);
	close(fd);
	if (written != (ssize_t)len)
	{
		unlink(filename);
		return (0);
	}
	return (1);
}

char	*create_heredoc_placeholder(int heredoc_id)
{
	char	*id_str;
	char	*placeholder;

	id_str = ft_itoa(heredoc_id);
	if (!id_str)
		return (NULL);
	placeholder = ft_strjoin("__HEREDOC_", id_str);
	free(id_str);
	return (placeholder);
}

int	count_heredocs(t_token *token)
{
	t_token	*current;
	int		count;

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

int	preprocess_heredocs(t_base *base)
{
	int heredoc_count;

	if (!base || !base->token)
		return (1);
	heredoc_count = count_heredocs(base->token);
	if (heredoc_count == 0)
		return (1);
	base->heredoc_count = heredoc_count;
	base->heredocs = NULL;
	if (!collect_heredocs_child(base->token, base))
		return (0);
	return (1);
}