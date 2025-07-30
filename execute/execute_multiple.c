/* ********************	pid = fork();
	if (pid == 0)
	{
		if (cmd_index > 0 && !has_input_redirection(cmd))
			dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
		if (cmd_index < data->cmd_count - 1 && !has_output_redirection(cmd))
			dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
		// ...existing code...************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 16:15:20 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static int	execute_child_process(t_token *cmd, t_exec_data *data,
		int cmd_index)
{
	t_token	*tmp;
	pid_t	pid;
	for (int i = 0; i < data->cmd_count; i++)
{
    printf("Command %d: ", i);
    t_token *current = data->commands[i];
    while (current)
    {
        printf("%s ", current->content);
        current = current->next;
    }
    printf("\n");
}
	pid = fork();
	if (pid == 0)
	{
		if (cmd_index > 0 && !has_input_redirection(cmd))
			dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
		if (cmd_index < data->cmd_count - 1 && !has_output_redirection(cmd))
			dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
		handle_redirections(cmd);
		cleanup_pipes(data->pipes, data->pipe_count);
		tmp = data->base->token;
		data->base->token = cmd;
		data->base->exit_status = single_execute_command(data->base);
		free_child_arg(data);
		free_tokens(tmp);
		exit(data->base->exit_status);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

static int	init_execution_resources(t_exec_data *data,
		t_base *base)
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
		free_commands(data->commands);
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
	// Parent process'te pipe'ları kapat
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

void	init_exec_data(t_exec_data *data)
{
	data->pipes = NULL;
	data->commands = NULL;
	data->pids = NULL;
	data->cmd_count = 0;
	data->pipe_count = 0;
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
		free_commands(data.commands);
		if (data.pids)
			free(data.pids);
		return (1);
	}
	cleanup_pipes(data.pipes, data.pipe_count);
	data.pipes = NULL;
	exit_status = wait_for_children(&data);
	free_commands(data.commands);
	data.commands = NULL;
	if (data.pids)
	{
		free(data.pids);
		data.pids = NULL;
	}
	return (exit_status);
}
