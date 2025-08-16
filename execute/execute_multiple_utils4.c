/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:45:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 16:02:12 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

void	setup_child_redirections(t_token *cmd, t_exec_data *data,
		int cmd_index)
{
	int	ctrl;

	if (should_skip_command_execution(data, cmd_index))
	{
		cleanup_pipes(data->pipes, data->pipe_count);
		free_child_arg(data);
		exit(0);
	}
	if (cmd_index > 0 && !has_input_redirection(cmd))
		dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < data->cmd_count - 1 && !has_output_redirection(cmd))
		dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
	ctrl = handle_redirections(cmd, data->base);
	redirect_control(data, ctrl);
	cleanup_pipes(data->pipes, data->pipe_count);
	data->base->token = cmd;
	data->base->exit_status = single_execute_command(data->base);
	free_child_arg(data);
	exit(data->base->exit_status);
}
