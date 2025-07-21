/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/21 19:34:52 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	execute_count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	fill_envp_array(t_env *env, char **envp, t_gc *gc)
{
	t_env	*current;
	char	*temp;
	int		i;

	i = 0;
	current = env;
	while (current)
	{
		temp = ft_strjoin_gc(gc, current->key, "=");
		envp[i] = ft_strjoin_gc(gc, temp, current->value);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
}

char	**env_to_envp(t_env *env, t_gc *gc)
{
	char	**envp;
	int		count;

	count = execute_count_env_vars(env);
	envp = (char **)ft_malloc_tmp(gc, sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	fill_envp_array(env, envp, gc);
	return (envp);
}
