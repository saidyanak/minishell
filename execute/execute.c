/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/21 20:22:16 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Forward declarations
static void	free_argv(char **argv);

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}
int	check_build_in(char *input)
{
	return (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0
		|| ft_strcmp(input, "env") == 0 || ft_strcmp(input, "pwd") == 0
		|| ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0
		|| ft_strcmp(input, "export") == 0 || ft_strcmp(input, "list") == 0);
}

// argv array'ini temizle
static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
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

	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	path_env = get_env_value(*base, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, command));
}

void	execute_command(t_base *base)
{
	t_token	*current_promt;

	current_promt = base->token;
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(current_promt->content))
			ft_build_in(current_promt, base);
		else
		{
			execute_external_command(current_promt, base);
		}
	}
}

