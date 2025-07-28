/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/22 12:23:20 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int	count_segments(t_token *token)
{
	t_token	*current;
	int		count;

	count = 1;
	current = token;
	while (current)
	{
		if (is_special_token(current->type))
			count++;
		current = current->next;
	}
	return (count);
}

static void	add_token_to_segment(t_token **head, t_token *original)
{
	char	*content;

	if (!original || is_special_token(original->type))
		return ;
	content = ft_strdup(original->content);
	if (!content)
		return ;
	add_token(head, content, original->type, original->q_type);
}

t_token	*copy_prompt_segment(t_token *start, t_token *end)
{
	t_token	*new_head;
	t_token	*original;

	if (!start)
		return (NULL);
	new_head = NULL;
	original = start;
	while (original && original != end)
	{
		add_token_to_segment(&new_head, original);
		original = original->next;
	}
	return (new_head);
}

static void	fill_segments(t_token *token, t_token **segments, int count)
{
	t_token	*current;
	t_token	*segment_start;
	int		i;

	current = token;
	segment_start = current;
	i = 0;
	while (current && i < count)
	{
		if (is_special_token(current->type))
		{
			segments[i] = copy_prompt_segment(segment_start, current);
			i++;
			if (current->type == TOKEN_PIPE)
			{
				current = current->next;
				segment_start = current;
				continue ;
			}
			else
			{
				current = current->next;
				if (current)
					current = current->next;
				segment_start = current;
				continue ;
			}
		}
		current = current->next;
	}
	if (segment_start && i < count)
		segments[i] = copy_prompt_segment(segment_start, NULL);
}

static t_token	**split_into_segments(t_token *token, int *segment_count)
{
	t_token	**segments;
	int		count;

	count = count_segments(token);
	segments = malloc(sizeof(t_token *) * (count + 1));
	if (!segments)
		return (NULL);
	fill_segments(token, segments, count);
	segments[count] = NULL;
	*segment_count = count;
	return (segments);
}

static void	handle_output_redirect(t_token *current)
{
	int	fd;

	if (current->type == TOKEN_REDIRECT_OUT && current->next)
	{
		fd = open(current->next->content, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else
		{
			perror(current->next->content);
		}
	}
	else if (current->type == TOKEN_APPEND && current->next)
	{
		fd = open(current->next->content, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else
		{
			perror(current->next->content);
		}
	}
}

static void	handle_input_redirect(t_token *current)
{
	int	fd;

	if (current->type == TOKEN_REDIRECT_IN && current->next)
	{
		fd = open(current->next->content, O_RDONLY);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	else if (current->type == TOKEN_HEREDOC && current->next)
	{
		if (setup_heredoc_input(current->next->content) == -1)
			perror("heredoc");
	}
}

static void	handle_redirections(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		handle_output_redirect(current);
		handle_input_redirect(current);
		current = current->next;
	}
}

static int	create_pipes(int ***pipes, int pipe_count)
{
	int	i;

	*pipes = malloc(sizeof(int *) * pipe_count);
	if (!*pipes)
		return (-1);
	i = 0;
	while (i < pipe_count)
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		if (pipe((*pipes)[i]) == -1)
		{
			perror("pipe");
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	close_unused_pipes(int **pipes, int pipe_count, int current_index)
{
	int	j;

	j = 0;
	while (j < pipe_count)
	{
		if (j != current_index - 1)
			close(pipes[j][0]);
		if (j != current_index)
			close(pipes[j][1]);
		j++;
	}
}

static void	setup_child_io(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

static void	execute_child_process(t_token *original_token,
		t_token *prompt_segment, t_base *base, int input_fd, int output_fd)
{
	int	result;

	setup_child_io(input_fd, output_fd);
	handle_redirections(original_token);
	result = execute_copy_prompt(prompt_segment, base);
	exit(result);
}

static void	create_child_processes(t_token **segments, t_token *token,
		t_base *base, int **pipes, pid_t *pids)
{
	int		i;
	int		segment_count;
	int		input_fd;
	int		output_fd;
	t_token	*original_segment;

	segment_count = 0;
	while (segments[segment_count])
		segment_count++;
	i = 0;
	while (i < segment_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			input_fd = (i > 0) ? pipes[i - 1][0] : STDIN_FILENO;
			output_fd = (i < segment_count - 1) ? pipes[i][1] : STDOUT_FILENO;
			close_unused_pipes(pipes, segment_count - 1, i);
			original_segment = find_original_segment(token, i);
			execute_child_process(original_segment, segments[i], base, input_fd,
				output_fd);
		}
		else if (pids[i] < 0)
			perror("fork");
		i++;
	}
}

static void	parent_cleanup_and_wait(int **pipes, pid_t *pids, int pipe_count,
		t_base *base)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pipe_count && WIFEXITED(status))
			base->exit_status = WEXITSTATUS(status);
		i++;
	}
}

static void	cleanup_memory(int **pipes, pid_t *pids, t_token **segments,
		int counts[2])
{
	int	i;

	i = 0;
	while (i < counts[0])
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
	free_segments(segments, counts[1]);
}

void			multiple_execute_command(t_token *token, t_base *base)
{
	t_token **segments;
	int segment_count;
	int pipe_count;
	int **pipes;
	pid_t *pids;
	int counts[2];

	segments = split_into_segments(token, &segment_count);
	if (!segments)
		return ;
	pipe_count = segment_count - 1;
	if (pipe_count <= 0)
	{
		execute_child_process(token, segments[0], base, STDIN_FILENO,
			STDOUT_FILENO);
		free_segments(segments, segment_count);
		return ;
	}
	if (create_pipes(&pipes, pipe_count) == -1)
		return ;
	pids = malloc(sizeof(pid_t) * segment_count);
	create_child_processes(segments, token, base, pipes, pids);
	parent_cleanup_and_wait(pipes, pids, pipe_count, base);
	counts[0] = pipe_count;
	counts[1] = segment_count;
	cleanup_memory(pipes, pids, segments, counts);
}