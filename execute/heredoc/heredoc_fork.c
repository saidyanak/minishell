/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:58:40 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cleanup_child_resources(t_base *base)
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

int	handle_child_process(int pipefd[2], char *delimiter, t_base *base)
{
	char	*content;

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

int	handle_parent_process(int pipefd[2], pid_t pid, t_base *base)
{
	int	status;

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

char	*read_from_pipe(int fd)
{
	char	*content;
	char	*temp;
	char	buffer[1024];
	ssize_t	bytes_read;

	content = malloc(1);
	if (!content)
		return (NULL);
	content[0] = '\0';
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	}
	return (content);
}

char	*run_heredoc_child_to_file(char *delimiter, t_base *base,
		t_heredoc_info *info_node)
{
	int		pipefd[2];
	pid_t	pid;
	char	*content;

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
