/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:45:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 15:31:50 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
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

static void	child_process(t_exec_params *params, t_base *base)
{
	struct stat	file_stat;

	setup_child_signals();
	if (stat(params->command_path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(params->command_path, STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			cleanup_all(base);
			cleanup_execution(params);
			exit(126);
		}
	}
	execve(params->command_path, params->argv, params->envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(params->command_path, STDERR_FILENO);
	if (access(params->command_path, F_OK) != 0)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (access(params->command_path, X_OK) != 0)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else
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
		child_process(&params, base);
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
