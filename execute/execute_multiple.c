/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 12:54:41 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	execute_child_process(t_token *cmd, t_exec_data *data,
		int cmd_index)
{
	pid_t	pid;
	int 	ctrl;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (cmd_index > 0 && !has_input_redirection(cmd))
			dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
		if (cmd_index < data->cmd_count - 1 && !has_output_redirection(cmd))
			dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
		ctrl = handle_redirections(cmd, data->base);
		redirect_control(data, ctrl);
		cleanup_pipes(data->pipes, data->pipe_count);
		data->base->token = cmd;
		data->base->exit_status = single_execute_command(data->base);
		cleanup_heredocs(data->base);
		free_child_arg(data);
		exit(data->base->exit_status);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

static int	init_execution_resources(t_exec_data *data, t_base *base)
{
	data->cmd_count = count_commands(base->token);
	data->pipe_count = data->cmd_count - 1;
	data->base = base;
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
	i = 0;
	while (i < data->pipe_count)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
	return (1);
}

static int	wait_for_children(t_exec_data *data)
{
	int	status;
	int	last_exit_status;

	setup_execution_signals();
	status = 0;
	last_exit_status = 0;
	last_exit_status = wait_for_children_utils(last_exit_status, status, data);
	setup_interactive_signals();
	return (last_exit_status);
}

int	execute_multiple_command(t_base *base)
{
	t_exec_data	data;
	int			exit_status;

	init_exec_data(&data);
	if (!init_execution_resources(&data, base))
		return (1);
	if (!launch_child_processes(&data))
	{
		cleanup_pipes(data.pipes, data.pipe_count);
		free_tokens_safe(&data);
		free_pids(&data);
		return (1);
	}
	cleanup_pipes(data.pipes, data.pipe_count);
	data.pipes = NULL;
	exit_status = wait_for_children(&data);
	free_tokens_safe(&data);
	free_pids(&data);
	return (exit_status);
}
