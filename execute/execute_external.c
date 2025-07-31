/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:45:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/31 18:26:04 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	cleanup_execution(t_exec_params *params)
{
	if (params->command_path)
		free(params->command_path);
	if (params->argv)
		free_string_array(params->argv);
	if (params->envp)
		free_string_array(params->envp);
}

static void	child_process(t_exec_params *params)
{
	execve(params->command_path, params->argv, params->envp);
	perror("minishell");
	exit(126);
}

static int	parent_process(pid_t pid, t_exec_params *params)
{
	int	status;

	waitpid(pid, &status, 0);
	cleanup_execution(params);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	execute_external_command(t_base *base)
{
	pid_t			pid;
	t_exec_params	params;
	int				prep_result;

	prep_result = prepare_execution(base->token, base, &params);
	if (prep_result != 0)
		return (prep_result);
	pid = fork();
	if (pid == 0)
		child_process(&params);
	else if (pid > 0)
		return (parent_process(pid, &params));
	else
	{
		perror("fork");
		cleanup_execution(&params);
		return (1);
	}
	return (0);
}
