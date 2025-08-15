/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 17:46:08 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_last_heredoc(t_token *cmd)
{
	t_token	*current;
	t_token	*last_heredoc;

	last_heredoc = NULL;
	current = cmd;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_HEREDOC)
			last_heredoc = current;
		current = current->next;
	}
	return (last_heredoc);
}

int	handle_heredoc_redirection(t_token *current)
{
	int	fd;

	fd = open(current->next->content, O_RDONLY);
	if (fd == -1)
	{
		perror("open heredoc");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	process_redirection_token(t_token *current, int fd,
		t_token *last_heredoc)
{
	int	error; 

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
	else if (current->type == TOKEN_HEREDOC && current == last_heredoc)
	{
		if (handle_heredoc_redirection(current) == -1)
			return (-1);
	}
	return (0);
}
