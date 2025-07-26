/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/26 16:04:55 by yuocak           ###   ########.fr       */
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
	commands[cmd_index] = current;
	while (current && cmd_index < cmd_count)
	{
		if (current->type == TOKEN_PIPE)
		{
			// Pipe token'ından önceki command'ı sonlandır (next = NULL yaparak)
			// Bu sayede her command sadece kendi token'larını görür
			temp = current->next;
			current->next = NULL;
			cmd_index++;
			if (cmd_index < cmd_count && temp)
				commands[cmd_index] = temp;
			current = temp;
			continue;
		}
		current = current->next;
	}
	commands[cmd_count] = NULL;
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
