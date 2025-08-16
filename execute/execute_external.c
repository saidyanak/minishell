/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:45:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 19:23:18 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
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
	setup_child_signals();
	execve(params->command_path, params->argv, params->envp);
	perror("minishell");
}

static int	parent_process(pid_t pid, t_exec_params *params)
{
	int	status;

	setup_execution_signals();
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	cleanup_execution(params);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(STDERR_FILENO, "\n", 1);
			return (130);
		}
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			return (131);
		}
	}
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
	{
		child_process(&params);
		cleanup_all(base);
		cleanup_execution(&params);
		exit(126);
	}
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
