/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 14:30:00 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void	heredoc_child_init(int pipefd[2], char *delimiter, t_base *base,
		t_heredoc_data *data)
{
	close(pipefd[0]);
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	data->should_expand = should_expand_delimiter(delimiter);
	data->clean_delimiter = remove_quotes_from_delimiter(delimiter);
	data->line_count = 1;
	data->base = base;
	data->pipefd = pipefd[1];
}

int	heredoc_check_exit_conditions(char *line, char *clean_delimiter,
		t_heredoc_data *data)
{
	if (*heredoc_static_flag(0) == 1)
	{
		if (line)
			free(line);
		free(clean_delimiter);
		close(data->pipefd);
		cleanup_child_resources(data->base);
		exit(130);
	}
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document at line ", 2);
		ft_putnbr_fd(data->line_count, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(clean_delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	if (ft_strcmp(line, clean_delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

void	heredoc_process_line(char *line, t_heredoc_data *data)
{
	char	*expanded;

	expanded = expand_heredoc_line(line, data->base, data->should_expand);
	write(data->pipefd, expanded, ft_strlen(expanded));
	write(data->pipefd, "\n", 1);
	free(line);
	free(expanded);
	data->line_count++;
}

void	heredoc_child_process(int pipefd[2], char *delimiter, t_base *base)
{
	t_heredoc_data	data;
	char			*line;

	heredoc_child_init(pipefd, delimiter, base, &data);
	while (1)
	{
		heredoc_static_flag(-1);
		line = readline("> ");
		if (*heredoc_static_flag(0) == 1)
		{
			if (line)
				free(line);
			free(data.clean_delimiter);
			close(data.pipefd);
			cleanup_child_resources(base);
			exit(130);
		}
		if (heredoc_check_exit_conditions(line, data.clean_delimiter, &data))
			break ;
		heredoc_process_line(line, &data);
	}
	free(data.clean_delimiter);
	close(data.pipefd);
	cleanup_child_resources(base);
	heredoc_static_flag(-1);
	exit(0);
}

char	*process_heredoc(char *delimiter, t_base *base)
{
	int		pipefd[2];
	pid_t	pid;
	char	*content;

	heredoc_static_flag(-1);
	if (pipe(pipefd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
		heredoc_child_process(pipefd, delimiter, base);
	else if (pid > 0)
	{
		content = read_heredoc_content(pipefd, pid, base);
		if (base->exit_status == 130)
		{
			rl_cleanup_after_signal();
			rl_reset_after_signal();
			rl_initialize();
			return (NULL);
		}
		return (content);
	}
	else
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}
	return (NULL);
}