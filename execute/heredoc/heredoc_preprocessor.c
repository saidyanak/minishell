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
/*   Created: 2025/08/06 12:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/06 17:00:00 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>


/* Header dosyasında t_heredoc_info struct'ını güncelle:
typedef struct s_heredoc_info
{
	char			*temp_filename;     // content yerine filename
	char			*original_delimiter;
	int				heredoc_id;
}					t_heredoc_info;
*/

static char	*create_temp_filename(int heredoc_id)
{
	char *id_str;
	char *temp_name;
	char *full_path;

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

static int	write_content_to_temp_file(char *filename, char *content)
{
	int fd;
	ssize_t written;
	size_t len;

	if (!content)
		return (1); // Boş content OK

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
		unlink(filename); // Başarısızlık durumunda dosyayı sil
		return (0);
	}
	return (1);
}

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
		array[i].temp_filename = NULL;
		array[i].original_delimiter = NULL;
		array[i].heredoc_id = i;
		i++;
	}
	return (array);
}

char	*run_heredoc_child_to_file(char *delimiter, t_base *base,
		t_heredoc_info *info)
{
	int pipefd[2];
	pid_t pid;
	int expand;
	char *content;
	int status;

	expand = should_expand_heredoc(delimiter);
	if (pipe(pipefd) == -1)
		return (NULL);

	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}

	if (pid == 0)
	{
		close(pipefd[0]);
		setup_heredoc_signals();
		content = read_heredoc_input_child(delimiter, base, expand);
		if (content)
		{
			write(pipefd[1], content, ft_strlen(content));
			free(content);
		}
		cleanup_failed_heredocs(info, base->heredoc_count);
		cleanup_all(base);
		close(pipefd[1]);
		exit(0);
	}

	close(pipefd[1]);
	waitpid(pid, &status, 0);

	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		base->exit_status = 130;
		return (NULL);
	}

	content = read_from_pipe(pipefd[0]);
	close(pipefd[0]);
	return (content);
}

char	*read_from_pipe(int fd)
{
	char *content;
	char *temp;
	char buffer[1024];
	ssize_t bytes_read;

	content = malloc(1);
	if (!content)
		return (NULL);
	content[0] = '\0';

	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
	}
	return (content);
}

void	cleanup_failed_heredocs(t_heredoc_info *heredocs, int count)
{
	int i;

	if (!heredocs)
		return ;
	i = 0;
	while (i < count)
	{
		if (heredocs[i].temp_filename)
		{
			unlink(heredocs[i].temp_filename); // Dosyayı sil
			free(heredocs[i].temp_filename);
			heredocs[i].temp_filename = NULL;
		}
		if (heredocs[i].original_delimiter)
		{
			free(heredocs[i].original_delimiter);
			heredocs[i].original_delimiter = NULL;
		}
		i++;
	}
	free(heredocs);
}

static int	process_single_heredoc_child(t_token *heredoc_token,
		t_heredoc_info *info, t_base *base)
{
	char *placeholder;
	char *content;

	if (!heredoc_token->next || !heredoc_token->next->content)
		return (0);

	info->original_delimiter = ft_strdup(heredoc_token->next->content);
	if (!info->original_delimiter)
		return (0);

	info->temp_filename = create_temp_filename(info->heredoc_id);
	if (!info->temp_filename)
	{
		free(info->original_delimiter);
		info->original_delimiter = NULL;
		return (0);
	}

	content = run_heredoc_child_to_file(heredoc_token->next->content, base,
			info);
	if (!content)
	{
		free(info->temp_filename);
		free(info->original_delimiter);
		info->temp_filename = NULL;
		info->original_delimiter = NULL;
		return (0);
	}

	if (!write_content_to_temp_file(info->temp_filename, content))
	{
		free(content);
		free(info->temp_filename);
		free(info->original_delimiter);
		info->temp_filename = NULL;
		info->original_delimiter = NULL;
		return (0);
	}

	free(content);

	placeholder = create_heredoc_placeholder(info->heredoc_id);
	if (!placeholder)
	{
		unlink(info->temp_filename);
		free(info->temp_filename);
		free(info->original_delimiter);
		info->temp_filename = NULL;
		info->original_delimiter = NULL;
		return (0);
	}

	free(heredoc_token->next->content);
	heredoc_token->next->content = placeholder;
	return (1);
}

static int	collect_heredocs_child(t_token *token, t_heredoc_info *heredocs,
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
			if (!process_single_heredoc_child(current, &heredocs[heredoc_index],
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

int	preprocess_heredocs(t_base *base)
{
	int heredoc_count;
	t_heredoc_info *heredocs;

	if (!base || !base->token)
		return (1);

	heredoc_count = count_heredocs(base->token);
	if (heredoc_count == 0)
		return (1);
	base->heredoc_count = heredoc_count;
	heredocs = create_heredoc_info_array(heredoc_count);
	if (!heredocs)
		return (0);

	if (!collect_heredocs_child(base->token, heredocs, base))
		return (0);

	base->heredocs = heredocs;
	return (1);
}