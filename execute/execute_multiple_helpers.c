/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/03 13:34:54 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	wait_for_children_utils(int last_exit_status, int status, t_exec_data *data)
{
	int	i;

	i = -1;
	while (++i < data->cmd_count)
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

void	free_tokens_safe(t_exec_data *data)
{
	int	i;

	i = -1;
	if (!data || !data->commands)
		return ;
	while (data->commands[++i])
		free_tokens(data->commands[i]);
	free_commands(data->commands);
	data->commands = NULL;
}

void	free_pids(t_exec_data *data)
{
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
}
