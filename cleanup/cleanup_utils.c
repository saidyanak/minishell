/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:31:43 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/07 15:40:40 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_commands(t_token **commands)
{
	if (!commands)
		return ;
	free(commands);
}

void	cleanup_all(t_base *base)
{
	if (!base)
		return ;
	// Token'ları temizle
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	// Heredoc'ları temizle
	cleanup_heredocs(base);
	// Environment'ı temizle
	if (base->env)
	{
		free_env_list(base->env);
		base->env = NULL;
	}
}

void	free_child_arg(t_exec_data *data)
{
	int	i;

	if (!data)
		return ;
	// Child process'te heredoc'ları temizle
	if (data && data->base)
		cleanup_heredocs(data->base);
	// Environment'ı temizle
	if (data->base && data->base->env)
	{
		free_env_list(data->base->env);
		data->base->env = NULL;
	}
	// Command'ları temizle
	if (data->commands)
	{
		i = 0;
		while (data->commands[i])
		{
			free_tokens(data->commands[i]);
			i++;
		}
		free_commands(data->commands);
		data->commands = NULL;
	}
	// PID'leri temizle
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
}

void	cleanup_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		i++;
	}
	free(pipes);
}

void	free_pids(t_exec_data *data)
{
	if (!data)
		return ;
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
}