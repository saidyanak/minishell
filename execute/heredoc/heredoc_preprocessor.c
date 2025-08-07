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
/*   Updated: 2025/08/07 19:00:00 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>


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

static t_heredoc_info	*create_heredoc_node(int heredoc_id)
{
	t_heredoc_info *node;

	node = malloc(sizeof(t_heredoc_info));
	if (!node)
		return (NULL);
	node->temp_filename = NULL;
	node->original_delimiter = NULL;
	node->heredoc_id = heredoc_id;
	node->next = NULL;
	return (node);
}

static void	add_heredoc_to_list(t_heredoc_info **head,
		t_heredoc_info *new_node)
{
	t_heredoc_info *current;

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

static t_heredoc_info	*find_heredoc_by_id(t_heredoc_info *head, int id)
{
	t_heredoc_info *current;

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
	t_heredoc_info *current;
	t_heredoc_info *next;

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

static void	cleanup_child_resources(t_base *base)
{
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	if (base->env)
	{
		free_env_list(base->env);
		base->env = NULL;
	}
	if (base->heredocs)
	{
		cleanup_heredocs_list(base->heredocs);
		base->heredocs = NULL;
	}
}

static int	handle_child_process(int pipefd[2], char *delimiter, t_base *base)
{
	char *content;

	close(pipefd[0]);
	setup_heredoc_signals();
	content = read_heredoc_input_child(delimiter, base, 0);
	if (content)
	{
		write(pipefd[1], content, ft_strlen(content));
		free(content);
	}
	cleanup_child_resources(base);
	close(pipefd[1]);
	exit(0);
}

static int	handle_parent_process(int pipefd[2], pid_t pid, t_base *base)
{
	int status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		base->exit_status = 130;
		return (0);
	}
	return (1);
}

char	*run_heredoc_child_to_file(char *delimiter, t_base *base,
		t_heredoc_info *info_node)
{
	int pipefd[2];
	pid_t pid;
	char *content;

	(void)info_node;
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
		handle_child_process(pipefd, delimiter, base);
	if (!handle_parent_process(pipefd, pid, base))
		return (NULL);
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

static int	setup_heredoc_node(t_token *heredoc_token,
		t_heredoc_info *info_node)
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

static int	create_temp_file_from_content(t_heredoc_info *info_node,
		char *content)
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

static int	create_placeholder_token(t_token *heredoc_token,
		t_heredoc_info *info_node)
{
	char *placeholder;

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

static int	process_single_heredoc_child(t_token *heredoc_token,
		t_heredoc_info *info_node, t_base *base)
{
	char *content;

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

static int	create_heredoc_list(t_token *token, t_base *base)
{
	t_token *current;
	t_heredoc_info *new_node;
	int heredoc_index;

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

static int	process_heredoc_list(t_token *token, t_base *base)
{
	t_token *current;
	t_heredoc_info *node;
	int heredoc_index;

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

static int	collect_heredocs_child(t_token *token, t_base *base)
{
	if (!create_heredoc_list(token, base))
		return (0);
	return (process_heredoc_list(token, base));
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