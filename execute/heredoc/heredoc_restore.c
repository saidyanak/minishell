/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_restore.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:59:31 by syanak            #+#    #+#             */
/*   Updated: 2025/08/06 17:07:28 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

static void	setup_heredoc_input(t_token *heredoc_token, t_base *base)
{
	int		heredoc_id;
	int		fd;
	char	*temp_filename;

	if (!heredoc_token->next || !heredoc_token->next->content)
		return ;
	if (!is_heredoc_placeholder(heredoc_token->next->content))
		return ;
	heredoc_id = extract_heredoc_id(heredoc_token->next->content);
	if (heredoc_id < 0 || heredoc_id >= base->heredoc_count)
		return ;
	temp_filename = base->heredocs[heredoc_id].temp_filename;
	if (!temp_filename)
		return ;
	fd = open(temp_filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open heredoc temp file");
		return ;
	}
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
	int i;

	if (!base)
		return ;

	if (base->heredocs)
	{
		i = 0;
		while (i < base->heredoc_count)
		{
			if (base->heredocs[i].temp_filename)
			{
				unlink(base->heredocs[i].temp_filename); // Temp dosyayı sil
				free(base->heredocs[i].temp_filename);
				base->heredocs[i].temp_filename = NULL;
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
	}
	base->heredoc_count = 0;
}