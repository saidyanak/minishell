/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 16:01:47 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <errno.h>
#include <fcntl.h>

static int	init_execution_resources(t_exec_data *data, t_base *base)
{
	data->cmd_count = count_commands(base->token);
	data->pipe_count = data->cmd_count - 1;
	data->base = base;
	base->data = data;
	data->base->token = base->token;
	data->base->env = base->env;
	data->pipes = create_pipes(data->pipe_count);
	if (data->pipe_count > 0 && !data->pipes)
		return (0);
	data->commands = split_commands(base->token, data->cmd_count);
	if (!data->commands)
	{
		cleanup_pipes(data->pipes, data->pipe_count);
		return (0);
	}
	data->pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!data->pids)
	{
		cleanup_pipes(data->pipes, data->pipe_count);
		free_commands(data->commands);
		return (0);
	}
	return (1);
}

static int	execute_cleanup_and_wait(t_exec_data *data)
{
	int	exit_status;

	if (data->pipes)
	{
		free(data->pipes);
		data->pipes = NULL;
	}
	exit_status = wait_for_children(data);
	free_tokens_safe(data);
	free_pids(data);
	return (exit_status);
}

int	execute_multiple_command(t_base *base)
{
	t_exec_data	data;

	init_exec_data(&data);
	if (!init_execution_resources(&data, base))
	{
		cleanup_heredocs(base);
		return (1);
	}
	if (!launch_child_processes(&data))
	{
		cleanup_pipes(data.pipes, data.pipe_count);
		free_tokens_safe(&data);
		free_pids(&data);
		cleanup_heredocs(base);
		return (1);
	}
	base->data = NULL;
	return (execute_cleanup_and_wait(&data));
}
