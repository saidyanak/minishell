/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_argv_envp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:15:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/11 19:01:37 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

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

static void	fill_argv_array(t_token *token, char **argv, t_gc *gc)
{
	t_token	*current;
	int		i;

	i = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			argv[i++] = ft_strdup_gc(gc, current->content);
		else if (is_redirection_token(current->type))
			current = current->next;
		if (current)
			current = current->next;
	}
	argv[i] = NULL;
}

char	**build_argv_from_tokens(t_token *token, t_gc *gc)
{
	char	**argv;
	int		argc;

	argc = count_args_in_command(token);
	argv = (char **)ft_malloc_tmp(gc, sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	fill_argv_array(token, argv, gc);
	return (argv);
}

int	prepare_execution(t_token *token, t_base *base, t_exec_params *params)
{
	params->argv = build_argv_from_tokens(token, base->gc);
	if (!params->argv || !(params->argv)[0])
	{
		return (1);
	}
	params->command_path = find_command_path((params->argv)[0], base);
	if (!params->command_path)
	{
		printf("minishell: %s: command not found\n", (params->argv)[0]);
		return (127);
	}
	params->envp = env_to_envp(base->env, base->gc);
	return (0);
}
