/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/27 15:45:00 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	check_build_in(char *input)
{
	if (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0)
		return (1);
	if (ft_strcmp(input, "env") == 0 || ft_strcmp(input, "pwd") == 0)
		return (1);
	if (ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0)
		return (1);
	if (ft_strcmp(input, "export") == 0 || ft_strcmp(input, "list") == 0)
		return (1);
	return (0);
}

void	free_argv(char **argv)
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

	// printf("DEBUG: Looking for command: '%s'\n", command);
	// Sadece absolute path'ler için local check yap
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
	{
		if (access(command, F_OK) == 0)
		{
			//		printf("DEBUG: Found as absolute/relative path\n");
			return (ft_strdup(command));
		}
	}
	// printf("DEBUG: Searching in PATH...\n");
	path_env = get_env_value(*base, "PATH");
	// printf("DEBUG: PATH value: '%s'\n", path_env ? path_env : "NULL");
	if (!path_env)
	{
		printf("DEBUG: PATH is NULL - returning NULL\n");
		return (NULL);
	}
	// printf("DEBUG: PATH found, splitting...\n");
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		//	printf("DEBUG: ft_split failed!\n");
		return (NULL);
	}
	// printf("DEBUG: Searching in paths...\n");
	return (search_in_paths(paths, command));
}

int	single_execute_command(t_token *token, t_base *base)
{
	if (!token)
		return (1);
	if (token->type == TOKEN_WORD || token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(token->content))
		{
			ft_build_in(token, base);
			return (base->exit_status);
		}
		else
		{
			return (execute_external_command(token, base));
		}
	}
	return (0);
}

void	execute_command(t_base *base)
{
	if (!base->token)
		return ;
	if (!check_syntax_errors(base->token))
	{
		base->exit_status = 2;
		return ;
	}
	if (has_special_tokens(base->token))
	{
		base->exit_status = execute_multiple_command(base->token, base);
	}
	else
	{
		base->exit_status = single_execute_command(base->token, base);
	}
}
