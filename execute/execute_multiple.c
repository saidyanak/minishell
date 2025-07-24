/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/24 18:02:27 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <unistd.h>

static int	execute_child_process(t_token *cmd, t_exec_data *data,
		int cmd_index)
{
	pid_t	pid;
	pid = fork();
	if (pid == 0)
	{
		if (cmd_index > 0)
			dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
		if (cmd_index < data->cmd_count - 1)
			dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
		handle_redirections(cmd);
		cleanup_pipes(data->pipes, data->pipe_count);
		data->base->exit_status = single_execute_command(cmd, data->base);
		exit(data->base->exit_status);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

static int	init_execution_resources(t_token *token, t_exec_data *data,
		t_base *base)
{
	data->cmd_count = count_commands(token);
	data->pipe_count = data->cmd_count - 1;
	data->base = base;
	data->pipes = create_pipes(data->pipe_count);
	if (data->pipe_count > 0 && !data->pipes)
		return (0);
	data->commands = split_commands(token, data->cmd_count);
	if (!data->commands)
	{
		cleanup_pipes(data->pipes, data->pipe_count);
		return (0);
	}
	data->pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!data->pids)
	{
		cleanup_pipes(data->pipes, data->pipe_count);
		free(data->commands);
		return (0);
	}
	return (1);
}

static int	launch_child_processes(t_exec_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count)
	{
		data->pids[i] = execute_child_process(data->commands[i], data, i);
		if (data->pids[i] == -1)
		{
			while (--i >= 0)
				waitpid(data->pids[i], NULL, 0);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	wait_for_children(t_exec_data *data)
{
	int	i;
	int	status;
	int	last_exit_status;

	last_exit_status = 0;
	i = 0;
	while (i < data->cmd_count)
	{
		waitpid(data->pids[i], &status, 0);
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_exit_status = 128 + WTERMSIG(status);
		i++;
	}
	return (last_exit_status);
}

int	execute_multiple_command(t_token *token, t_base *base)
{
	t_exec_data	data;
	int			exit_status;

	if (!init_execution_resources(token, &data, base))
		return (1);
	if (!launch_child_processes(&data))
	{
		cleanup_pipes(data.pipes, data.pipe_count);
		free(data.commands);
		free(data.pids);
		return (1);
	}
	cleanup_pipes(data.pipes, data.pipe_count);
	exit_status = wait_for_children(&data);
	free(data.commands);
	free(data.pids);
	return (exit_status);
}
