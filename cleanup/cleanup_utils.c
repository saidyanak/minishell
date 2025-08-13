/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:31:43 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/13 20:45:22 by syanak           ###   ########.fr       */
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
	if (base->token != NULL)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
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
	if (data && data->base)
		cleanup_heredocs(data->base);
	if (data->base && data->base->env && data->base->env->key
		&& data->base->env->value)
	{
		free_env_list(data->base->env);
		data->base->env = NULL;
	}
	if (data->commands && data->base->token->content)
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