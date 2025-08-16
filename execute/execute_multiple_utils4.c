/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:45:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/17 00:00:00 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static void	execute_filtered_command(t_token *cmd, t_exec_data *data)
{
	t_token	*cmd_tokens;

	cmd_tokens = extract_command_tokens(cmd);
	if (cmd_tokens)
	{
		data->base->token = cmd_tokens;
		data->base->exit_status = single_execute_command(data->base);
		free_tokens(cmd_tokens);
	}
	else
		data->base->exit_status = 0;
}

void	setup_child_redirections(t_token *cmd, t_exec_data *data, int cmd_index)
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
	execute_filtered_command(cmd, data);
	free_child_arg(data);
	exit(data->base->exit_status);
}
