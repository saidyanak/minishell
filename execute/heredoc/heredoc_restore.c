/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_restore.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:59:31 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:48:42 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <fcntl.h>

int	open_heredoc_file(t_heredoc_info *heredoc_node)
{
	int	fd;

	if (!heredoc_node || !heredoc_node->temp_filename)
		return (-1);
	fd = open(heredoc_node->temp_filename, O_RDONLY);
	return (fd);
}

void	setup_heredoc_input(t_token *heredoc_token, t_base *base)
{
	int				heredoc_id;
	int				fd;
	t_heredoc_info	*heredoc_node;

	if (!heredoc_token->next || !heredoc_token->next->content)
		return ;
	if (!is_heredoc_placeholder(heredoc_token->next->content))
		return ;
	heredoc_id = extract_heredoc_id(heredoc_token->next->content);
	if (heredoc_id < 0)
		return ;
	heredoc_node = find_heredoc_by_id(base->heredocs, heredoc_id);
	fd = open_heredoc_file(heredoc_node);
	if (fd == -1)
		return ;
	dup2(fd, STDIN_FILENO);
	close(fd);
}

t_token	*find_last_heredoc(t_token *cmd)
{
	t_token	*current;
	t_token	*last_heredoc;

	current = cmd;
	last_heredoc = NULL;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			last_heredoc = current;
		current = current->next;
	}
	return (last_heredoc);
}

void	restore_heredocs_in_redirections(t_token *cmd, t_base *base)
{
	t_token	*last_heredoc;

	if (!base || !base->heredocs)
		return ;
	last_heredoc = find_last_heredoc(cmd);
	if (last_heredoc)
		setup_heredoc_input(last_heredoc, base);
}

char	*initialize_empty_content_safe(void)
{
	char	*content;

	content = malloc(1);
	if (!content)
		return (NULL);
	content[0] = '\0';
	return (content);
}
