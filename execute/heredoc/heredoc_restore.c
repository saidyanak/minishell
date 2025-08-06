/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_restore.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:59:31 by syanak            #+#    #+#             */
/*   Updated: 2025/08/06 09:40:45 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <fcntl.h>

static int	is_heredoc_placeholder(char *str)
{
	if (!str)
		return (0);
	return (ft_strncmp(str, "__HEREDOC_", 10) == 0);
}

static int	extract_heredoc_id(char *placeholder)
{
	char	*id_str;
	int		id;

	if (!placeholder || ft_strlen(placeholder) <= 10)
		return (-1);
	id_str = placeholder + 10;
	id = ft_atoi(id_str);
	return (id);
}

static int	write_heredoc_to_pipe(int write_fd, char *content)
{
	size_t	len;
	ssize_t	written;

	if (!content)
		return (0);
	len = ft_strlen(content);
	written = write(write_fd, content, len);
	if (written == -1)
	{
		perror("write");
		return (-1);
	}
	return (0);
}

static int	create_pipe_for_heredoc(char *content)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		write_heredoc_to_pipe(pipefd[1], content);
		close(pipefd[1]);
		exit(0);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

static void	setup_heredoc_input(t_token *heredoc_token, t_base *base)
{
	int		heredoc_id;
	int		fd;
	char	*content;

	if (!heredoc_token->next || !heredoc_token->next->content)
		return ;
	if (!is_heredoc_placeholder(heredoc_token->next->content))
		return ;
	heredoc_id = extract_heredoc_id(heredoc_token->next->content);
	if (heredoc_id < 0 || heredoc_id >= base->heredoc_count)
		return ;
	content = base->heredocs[heredoc_id].content;
	fd = create_pipe_for_heredoc(content);
	if (fd == -1)
		return ;
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	restore_heredocs_in_redirections(t_token *cmd, t_base *base)
{
	t_token	*current;

	if (!base || !base->heredocs)
		return ;
	current = cmd;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			setup_heredoc_input(current, base);
		current = current->next;
	}
}

void	cleanup_heredocs(t_base *base)
{
	int	i;

	if (!base)
		return ;
	if (!base->heredocs)
	{
		base->heredoc_count = 0;
		return ;
	}
	i = 0;
	while (i < base->heredoc_count)
	{
		if (base->heredocs[i].content)
		{
			free(base->heredocs[i].content);
			base->heredocs[i].content = NULL;
		}
		if (base->heredocs[i].original_delimiter)
		{
			free(base->heredocs[i].original_delimiter);
			base->heredocs[i].original_delimiter = NULL;
		}
		i++;
	}
	free(base->heredocs);
	base->heredocs = NULL;
	base->heredoc_count = 0;
}
