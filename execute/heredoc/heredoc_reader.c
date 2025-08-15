/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_reader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 14:30:00 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <sys/wait.h>
#include <unistd.h>

char	*init_heredoc_content(int pipefd[2], pid_t pid, t_base *base)
{
	char	*content;
	int		status;

	close(pipefd[1]);
	content = initialize_empty_content_safe();
	if (!content)
	{
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			base->exit_status = 130;
		return (NULL);
	}
	return (content);
}

char	*read_heredoc_buffer(int pipefd, char *content)
{
	char	buffer[1024];
	char	*temp;
	int		bytes_read;

	while ((bytes_read = read(pipefd, buffer, 1023)) > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		if (!temp)
		{
			free(content);
			return (NULL);
		}
		free(content);
		content = temp;
		temp = NULL;
	}
	return (content);
}

int	handle_child_exit_status(pid_t pid, t_base *base)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		base->exit_status = 130;
		return (1);
	}
	return (0);
}

char	*read_heredoc_content(int pipefd[2], pid_t pid, t_base *base)
{
	char	*content;

	content = init_heredoc_content(pipefd, pid, base);
	if (!content)
		return (NULL);
	content = read_heredoc_buffer(pipefd[0], content);
	if (!content)
	{
		close(pipefd[0]);
		handle_child_exit_status(pid, base);
		return (NULL);
	}
	close(pipefd[0]);
	if (handle_child_exit_status(pid, base))
	{
		free(content);
		return (NULL);
	}
	return (content);
}

t_heredoc_info	*create_heredoc_info(char *temp_file, char *delimiter)
{
	t_heredoc_info	*new_heredoc;

	new_heredoc = malloc(sizeof(t_heredoc_info));
	if (!new_heredoc)
		return (NULL);
	new_heredoc->temp_filename = ft_strdup(temp_file);
	new_heredoc->original_delimiter = ft_strdup(delimiter);
	new_heredoc->heredoc_id = get_unique_heredoc_id();
	new_heredoc->next = NULL;
	if (!new_heredoc->temp_filename || !new_heredoc->original_delimiter)
	{
		if (new_heredoc->temp_filename)
			free(new_heredoc->temp_filename);
		if (new_heredoc->original_delimiter)
			free(new_heredoc->original_delimiter);
		free(new_heredoc);
		return (NULL);
	}
	return (new_heredoc);
}