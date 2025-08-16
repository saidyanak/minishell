/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:35:52 by syanak            #+#    #+#             */
/*   Updated: 2025/08/16 17:25:46 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

char	*get_heredoc_content(t_token *token, t_base *base)
{
	char	*content;

	content = process_heredoc(token->next->content, base);
	if (!content && base->exit_status == 130)
		return (NULL);
	if (!content)
		content = ft_strdup("");
	return (content);
}

int	create_heredoc_file(char *content, char **temp_file)
{
	int	fd;

	*temp_file = create_temp_filename();
	if (!*temp_file)
	{
		free(content);
		return (0);
	}
	fd = open(*temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(content);
		free(*temp_file);
		return (0);
	}
	write(fd, content, ft_strlen(content));
	close(fd);
	free(content);
	return (1);
}

int	save_heredoc_info(char *temp_file, t_token *token, t_base *base)
{
	t_heredoc_info	*new_heredoc;

	new_heredoc = create_heredoc_info(temp_file, token->next->content);
	if (new_heredoc)
	{
		new_heredoc->next = base->heredocs;
		base->heredocs = new_heredoc;
		base->heredoc_count++;
		return (1);
	}
	return (0);
}

void	update_token_content(t_token *token, char *temp_file)
{
	free(token->next->content);
	token->next->content = temp_file;
}

int	handle_heredoc_token(t_token *token, t_base *base)
{
	char	*content;
	char	*temp_file;

	if (!token || !token->next || !token->next->content)
		return (0);
	content = get_heredoc_content(token, base);
	if (!content && base->exit_status == 130)
		return (0);
	if (!content)
		return (0);
	if (!create_heredoc_file(content, &temp_file))
		return (0);
	if (!save_heredoc_info(temp_file, token, base))
	{
		unlink(temp_file);
		free(temp_file);
		return (0);
	}
	update_token_content(token, temp_file);
	return (1);
}
