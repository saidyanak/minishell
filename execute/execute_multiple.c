/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/14 15:46:12 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>
#include <fcntl.h>

// Yeni yardımcı fonksiyon: sonraki komutun input redirection'ı olup olmadığını kontrol eder
static int	next_command_has_input_redirect(t_exec_data *data, int cmd_index)
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

// Yeni yardımcı fonksiyon: komutun çıktı üretip üretmeyeceğini kontrol eder
static int	should_skip_command_execution(t_exec_data *data, int cmd_index)
{
	// Eğer son komut değilse ve sonraki komutun input redirection'ı varsa
	if (cmd_index < data->cmd_count - 1 && next_command_has_input_redirect(data,
			cmd_index))
	{
		// Bu komutun çıktı redirection'ı yoksa (yani sadece pipe'a yazacaksa)
		if (!has_output_redirection(data->commands[cmd_index]))
			return (1);
	}
	return (0);
}

static int	execute_child_process(t_token *cmd, t_exec_data *data,
		int cmd_index)
{
	pid_t	pid;
	int		ctrl;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (should_skip_command_execution(data, cmd_index))
		{
			// Pipe'ları kapat ve çık
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

static int	launch_child_processes(t_exec_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count)
	{
		data->pids[i] = execute_child_process(data->commands[i], data, i);
		if (data->pids[i] == -1)
		{
			// Fork başarısız olursa önceki child'ları temizle
			while (--i >= 0)
				waitpid(data->pids[i], NULL, 0);
			return (0);
		}
		i++;
	}
	// CRITICAL: Tüm child'lar başlatıldıktan SONRA parent'taki pipe'ları kapat
	// Bu sayede child'lar arasındaki pipe iletişimi kesilmez
	i = 0;
	while (i < data->pipe_count)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		// Her pipe array'ini free et
		free(data->pipes[i]);
		data->pipes[i] = NULL;
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
	t_exec_data data;
	int exit_status;

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

	// CRITICAL: Parent process'te pipe array'ini free et
	// launch_child_processes içinde pipe'lar kapatıldı ama array free edilmedi
	if (data.pipes)
	{
		free(data.pipes);
		data.pipes = NULL;
	}

	exit_status = wait_for_children(&data);
	free_tokens_safe(&data);
	free_pids(&data);

	// Multiple command tamamlandıktan sonra heredoc'ları temizle
	cleanup_heredocs(base);
	return (exit_status);
}