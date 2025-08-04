/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 12:11:01 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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
}

void	free_child_arg(t_exec_data *data)
{
	int	i;

	i = -1;
	free_env_list(data->base->env);
	while (data->commands[++i])
		free_tokens(data->commands[i]);
	free_commands(data->commands);
	free(data->pids);
}
