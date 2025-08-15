/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 19:07:50 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int	next_command_has_input_redirect(t_exec_data *data, int cmd_index)
{
	t_token	*next_cmd;

	if (cmd_index >= data->cmd_count - 1)
		return (0);
	next_cmd = data->commands[cmd_index + 1];
	while (next_cmd)
	{
		if (next_cmd->type == TOKEN_REDIRECT_IN
			|| next_cmd->type == TOKEN_HEREDOC)
			return (1);
		if (next_cmd->type == TOKEN_PIPE)
			break ;
		next_cmd = next_cmd->next;
	}
	return (0);
}

int	should_skip_command_execution(t_exec_data *data, int cmd_index)
{
	if (cmd_index < data->cmd_count - 1 && next_command_has_input_redirect(data,
			cmd_index))
	{
		if (!has_output_redirection(data->commands[cmd_index]))
			return (1);
	}
	return (0);
}

int	launch_child_processes(t_exec_data *data)
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
		free(data->pipes[i]);
		data->pipes[i] = NULL;
		i++;
	}
	return (1);
}

int	wait_for_children(t_exec_data *data)
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

int	execute_child_process(t_token *cmd, t_exec_data *data, int cmd_index)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		setup_child_redirections(cmd, data, cmd_index);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}
