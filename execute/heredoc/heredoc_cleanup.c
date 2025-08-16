/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:34:21 by syanak            #+#    #+#             */
/*   Updated: 2025/08/16 16:34:22 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include <fcntl.h>
#include <unistd.h>

void	cleanup_heredocs(t_base *base)
{
	t_heredoc_info	*current;
	t_heredoc_info	*next;

	if (!base || !base->heredocs)
		return ;
	current = base->heredocs;
	while (current)
	{
		next = current->next;
		if (current->temp_filename)
		{
			free(current->temp_filename);
		}
		if (current->original_delimiter)
			free(current->original_delimiter);
		free(current);
		current = next;
	}
	base->heredocs = NULL;
	base->heredoc_count = 0;
}

void	process_all_heredocs(t_base *base)
{
	t_token	*current;

	if (!base || !base->token)
		return ;
	current = base->token;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (!handle_heredoc_token(current, base))
			{
				if (base->exit_status != 130)
					base->exit_status = 1;
				return ;
			}
		}
		current = current->next;
	}
}

void	restore_heredocs_in_redirections(t_token *cmd, t_base *base)
{
	t_token	*current;
	int		fd;

	(void)base;
	current = cmd;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC && current->next
			&& current->next->content)
		{
			fd = open(current->next->content, O_RDONLY);
			if (fd != -1)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		current = current->next;
	}
}

void	cleanup_child_resources(t_base *base)
{
	if (base->env)
		free_env_list(base->env);
	if (base->heredocs)
		cleanup_heredocs(base);
	if (base->token)
		free_tokens(base->token);
}

int	get_unique_heredoc_id(void)
{
	static int	counter = 0;

	return (++counter);
}
