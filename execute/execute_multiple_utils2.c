/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/27 17:17:46 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <unistd.h>

t_token	**split_commands(t_token *token, int cmd_count)
{
	t_token	**commands;
	t_token	*current;
	t_token	*temp;
	int		cmd_index;

	commands = malloc(sizeof(t_token *) * (cmd_count + 1));
	if (!commands)
		return (NULL);
	current = token;
	cmd_index = 0;
	commands[cmd_index++] = current;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			temp = current->next;
			current->next = NULL;
			if (cmd_index < cmd_count)
				commands[cmd_index++] = temp;
			current = temp;
		}
		else
			current = current->next;
	}
	commands[cmd_index] = NULL;
	return (commands);
}

void	cleanup_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		i++;
	}
	free(pipes);
}
