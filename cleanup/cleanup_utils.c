/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:31:43 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 15:51:43 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>

void	free_commands(t_token **commands)
{
	if (!commands)
		return ;
	free(commands);
}

void	cleanup_all(t_base *base)
{
	if (!base)
		return ;
	if (base->data == NULL)
	{
		cleanup_base_normal(base);
	}
	else if (base->data && base->data->commands)
	{
		cleanup_base_with_data(base);
	}
}

void	free_child_arg(t_exec_data *data)
{
	if (!data)
		return ;
	if (data && data->base)
		cleanup_heredocs(data->base);
	free_child_env_and_commands(data);
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
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

void	free_pids(t_exec_data *data)
{
	if (!data)
		return ;
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
}
