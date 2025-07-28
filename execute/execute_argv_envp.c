/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_argv_envp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:15:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/28 17:53:50 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	count_args_in_command(t_token *token)
{
	int		count;
	t_token	*current;

	count = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			count++;
		else if (is_redirection_token(current->type))
			current = current->next;
		if (current)
			current = current->next;
	}
	return (count);
}

static char	**fill_argv_array(t_token *token, char **argv)
{
	t_token	*current;
	int		i;

	i = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			argv[i] = ft_strdup(current->content);
			if (!argv[i++])
			{
				free_argv(argv);
				return (NULL);
			}
		}
		else if (is_redirection_token(current->type))
			current = current->next;
		if (current)
			current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

char	**build_argv_from_tokens(t_token *token)
{
	char	**argv;
	int		argc;

	argc = count_args_in_command(token);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv || !fill_argv_array(token, argv))
		return (NULL);
	return (argv);
}

int	prepare_execution(t_token *token, t_base *base, t_exec_params *params)
{
	params->argv = build_argv_from_tokens(token);
	if (!params->argv || !(params->argv)[0])
	{
		free_argv(params->argv);
		return (1);
	}
	params->command_path = find_command_path((params->argv)[0], base);
	if (!params->command_path)
	{
		printf("minishell: %s: command not found\n", (params->argv)[0]);
		free_argv(params->argv);
		return (127);
	}
	params->envp = env_to_envp(base->env);
	return (0);
}
