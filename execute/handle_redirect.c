/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:26:39 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/06 12:59:29 by syanak           ###   ########.fr       */
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

int	handle_redirections(t_token *cmd, t_base *base, t_exec_data *exec)
{
	t_token	*current;
	int		fd;
	int		error;

	fd = 0;
	current = cmd;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN)
		{
			error = redirect_in(current, fd);
			if (error == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIRECT_OUT)
		{
			error = redirect_out(current, fd);
			if (error == -1)
				return (-1);
		}
		else if (current->type == TOKEN_APPEND)
		{
			error = redirect_append(current, fd);
			if (error == -1)
				return (-1);
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			restore_heredocs_in_redirections(cmd, base, exec);
		}
		current = current->next;
	}
	return (0);
}
