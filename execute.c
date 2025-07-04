/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/04 15:46:44 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	check_build_in(char *input)
{
	return (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0
		|| ft_strcmp(input, "env") == 0 || ft_strcmp(input, "pwd") == 0
		|| ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0
		|| ft_strcmp(input, "export") == 0 || ft_strcmp(input, "list") == 0);
}

int	validate_path_and_command(char *path_env, )
{

}
int execute_external_command(t_token *current_prompt, t_base *base)
{
	char	**paths;
	char	*command_path;
	char	**argv;
	char	**envp;
	char	*path_env;

	path_env = getenv("PATH");
	if (!validate_path_and_command(path_env, current_prompt->content))
		return (-1);
	paths = ft_split(path_env, ':');
	command_path = find_command_in_paths(paths, current_prompt->content);
	if (!command_path)
	{
		printf("minishell: %s: command not found\n", current_prompt->content);
		free_string_array(paths);
		return (-1);
	}
	execute_command_with_path(command_path, argv, envp);
	cleanup_execution_resources(command_path, paths, argv, envp);
	return (0);
}

int single_execute_command(t_token *token, t_base *base)
{
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(token->content))
			ft_build_in(token, base);
		else
		{
			execute_external_command(token, base);
		}
	}
}

void	execute_command(t_base *base)
{
	t_token	*current_promt;

	current_promt = base->token;
	if (is_single_execute(current_promt))
		single_execute_command(current_promt, base);
	else
		multiple_execute_command(current_promt, base);

}
