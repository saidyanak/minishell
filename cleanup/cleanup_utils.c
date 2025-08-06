/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:31:43 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/06 16:40:20 by syanak           ###   ########.fr       */
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
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	if (base->env)
	{
		free_env_list(base->env);
		base->env = NULL;
	}
	cleanup_heredocs(base);
}

void	free_child_arg(t_exec_data *data)
{
	int	i;

	if (data && data->base)
		cleanup_heredocs(data->base);
	i = -1;
	free_env_list(data->base->env);
	while (data->commands[++i])
		free_tokens(data->commands[i]);
	free_commands(data->commands);
	free(data->pids);
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
	if (data->pids)
	{
		free(data->pids);
		data->pids = NULL;
	}
}
