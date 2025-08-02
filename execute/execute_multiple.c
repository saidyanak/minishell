/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/02 13:45:31 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static int	execute_child_process(t_token *cmd, t_exec_data *data,
		int cmd_index)
{
	pid_t	pid;
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (cmd_index > 0 && !has_input_redirection(cmd))
			dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
		if (cmd_index < data->cmd_count - 1 && !has_output_redirection(cmd))
			dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
		handle_redirections(cmd);
		cleanup_pipes(data->pipes, data->pipe_count);
		data->base->token = cmd;
		data->base->exit_status = single_execute_command(data->base);
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

	setup_execution_signals();
	last_exit_status = 0;
	i = 0;
	while (i < data->cmd_count)
	{
		waitpid(data->pids[i], &status, 0);
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				write(STDOUT_FILENO, "\n", 1);
				last_exit_status = 130;
			}
			else if (WTERMSIG(status) == SIGQUIT)
			{
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				last_exit_status = 131;
			}
			else
				last_exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	setup_interactive_signals();
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
void	free_tokens_safe(t_exec_data *data)
{
	int	i;

	i = -1;
	if (!data || !data->commands)
		return ;
	while (data->commands[++i])
		free_tokens(data->commands[i]); // her bir komutun tokenlarını sil
	free_commands(data->commands);      // t_token** dizisini sil
	data->commands = NULL;              // double free önlemek için
}
void	free_pids(t_exec_data *data)
{
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
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
