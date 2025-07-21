/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:45:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/19 12:01:42 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void	cleanup_execution(t_exec_params *params)
{
	(void)params;
	// command_path might not be GC-allocated if it's a direct path
	// argv and envp are GC-allocated, don't free manually
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
	return (1);
}

int	execute_external_command(t_token *token, t_base *base)
{
	pid_t			pid;
	t_exec_params	params;
	int				prep_result;

	prep_result = prepare_execution(token, base, &params);
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
