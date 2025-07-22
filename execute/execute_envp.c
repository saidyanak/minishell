/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/22 09:39:18 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

int	count_env_vars(t_env *env)
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

static void	fill_envp_array(t_env *env, char **envp)
{
	t_env	*current;
	char	*temp;
	int		i;

	i = 0;
	current = env;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		envp[i] = ft_strjoin(temp, current->value);
		free(temp);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
}

char	**env_to_envp(t_env *env)
{
	char	**envp;
	int		count;

	count = count_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	fill_envp_array(env, envp);
	return (envp);
}
