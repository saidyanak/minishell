/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path_finder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:24:13 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 15:01:33 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_absolute_or_relative_path(char *command)
{
	return (command[0] == '/' || (command[0] == '.' && command[1] == '/'));
}

static char	*check_absolute_path(char *command)
{
	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	return (NULL);
}

static char	*search_in_paths(char **paths, char *command)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_argv(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_argv(paths);
	return (NULL);
}

char	*find_command_path(char *command, t_base *base)
{
	char	*path_env;
	char	**paths;

	if (is_absolute_or_relative_path(command))
		return (check_absolute_path(command));
	path_env = get_value(base->env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, command));
}
