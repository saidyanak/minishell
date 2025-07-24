/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/24 15:22:42 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	count_commands(t_token *token)
{
	int		count;
	t_token	*current;

	count = 1;
	current = token;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

static void	cleanup_failed_pipes(int **pipes, int i)
{
	while (--i >= 0)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}
	free(pipes);
}

static int	create_single_pipe(int **pipes, int i)
{
	pipes[i] = malloc(sizeof(int) * 2);
	if (!pipes[i] || pipe(pipes[i]) == -1)
		return (0);
	return (1);
}

int	**create_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	if (pipe_count <= 0)
		return (NULL);
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		if (!create_single_pipe(pipes, i))
		{
			cleanup_failed_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}
