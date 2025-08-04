/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:26:39 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 09:01:35 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void	redirect_out(t_token *current, int fd)
{
	fd = open(current->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open >");
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	redirect_append(t_token *current, int fd)
{
	fd = open(current->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror("open >>");
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	redirect_in(t_token *current, int fd)
{
	fd = open(current->next->content, O_RDONLY);
	if (fd == -1)
		perror("open <");
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	handle_redirections(t_token *cmd, t_base *base)
{
	t_token *current;
	int fd;

	fd = 0;
	current = cmd;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN)
			redirect_in(current, fd);
		else if (current->type == TOKEN_REDIRECT_OUT)
			redirect_out(current, fd);
		else if (current->type == TOKEN_APPEND)
			redirect_append(current, fd);
		else if (current->type == TOKEN_HEREDOC)
			restore_heredocs_in_redirections(cmd, base);
		current = current->next;
	}
}