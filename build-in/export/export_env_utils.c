/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:14:49 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/21 20:28:29 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

t_env	*find_env_var(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*create_env_string(char *key, char *value)
{
	char	*env_str;
	char	*temp;

	if (!key)
		return (NULL);
	if (!value)
		return (ft_strdup(key));
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	env_str = ft_strjoin(temp, value);
	free(temp);
	return (env_str);
}

t_env	**create_env_array(t_env *env, int count)
{
	t_env	**array;
	int		i;

	array = malloc(sizeof(t_env *) * count);
	if (!array)
		return (NULL);
	i = 0;
	while (env && i < count)
	{
		array[i] = env;
		env = env->next;
		i++;
	}
	return (array);
}
