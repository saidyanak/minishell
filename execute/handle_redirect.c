/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:26:39 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 17:53:21 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_control(t_exec_data *data, int ctrl)
{
	if (ctrl == -1)
	{
		cleanup_pipes(data->pipes, data->pipe_count);
		free_child_arg(data);
		exit(1);
	}
}

int	redirect_out(t_token *current, int fd)
{
	fd = open(current->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open >");
		return (-1);
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int	redirect_append(t_token *current, int fd)
{
	fd = open(current->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open >>");
		return (-1);
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int	redirect_in(t_token *current, int fd)
{
	fd = open(current->next->content, O_RDONLY);
	if (fd == -1)
	{
		perror("open <");
		return (-1);
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	handle_redirections(t_token *cmd, t_base *base)
{
	int		error;
	t_token	*current;
	t_token	*last_heredoc;
	int		fd;

	(void)base;
	error = 0;
	fd = 0;
	last_heredoc = find_last_heredoc(cmd);
	current = cmd;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_REDIRECT_IN
			|| current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_APPEND
			|| (current->type == TOKEN_HEREDOC && current == last_heredoc))
		{
			if (process_redirection_token(current, fd, last_heredoc,
					error) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
