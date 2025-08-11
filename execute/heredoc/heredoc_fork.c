/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/09 17:13:35 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Setter fonksiyonu - base ve pipefd'yi set eder */
void	set_signal_data(t_base **base, int **pipefd, char **content)
{
	static t_base	**stored_base_ptr = NULL;
	static int		**stored_pipefd_ptr = NULL;
	static char		**free_content = NULL;

	if (!(*base) && !(*pipefd))
	{
		base = stored_base_ptr;
		pipefd = stored_pipefd_ptr;
		content = free_content;
		return ;
	}
	if (!stored_base_ptr)
		stored_base_ptr = malloc(sizeof(t_base *));
	if (!stored_pipefd_ptr)
		stored_pipefd_ptr = malloc(sizeof(int *));
	stored_base_ptr = base;
	stored_pipefd_ptr = pipefd;
}

/* Child process SIGINT handler */
static void	child_sigint_handler(int sig)
{
	t_base	*child_base;
	int		*child_pipefd;
	char	**free_content;

	free_content = NULL;
	child_base = NULL;
	child_pipefd = NULL;
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	child_base->exit_status = 130;
	/* Getter ile değerleri al */
	set_signal_data(&child_base, &child_pipefd, free_content);
	/* Pipe cleanup */
	if (child_pipefd && *child_pipefd != -1)
	{
		close(child_pipefd[1]);
		*child_pipefd = -1;
	}
	if (free_content)
		free(free_content);
	/* Base cleanup - NULL check çok önemli */
	if (child_base)
	{
		if (child_base->token)
		{
			free_tokens(child_base->token);
			child_base->token = NULL;
		}
		if (child_base->env)
		{
			free_env_list(child_base->env);
			child_base->env = NULL;
		}
		if (child_base->heredocs)
		{
			cleanup_heredocs_list(child_base->heredocs);
			child_base->heredocs = NULL;
		}
	}
	free(child_base);
	clear_history();
	exit(130);
}

/* Child process için signal setup */
void	setup_fork_signal(t_base *base, int *pipefd, char **content)
{
	/* Setter ile değerleri set et */
	set_signal_data(&base, &pipefd, content);
	/* Signal handler'ları ayarla */
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/* Normal cleanup - signal olmadan çıkış için */
void	cleanup_child_resources(t_base *base)
{
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	if (base->env)
	{
		free_env_list(base->env);
		base->env = NULL;
	}
	if (base->heredocs)
	{
		cleanup_heredocs_list(base->heredocs);
		base->heredocs = NULL;
	}
}

/* Updated child process handler */
int	handle_child_process(int pipefd[2], char *delimiter, t_base *base)
{
	char	*content;

	close(pipefd[0]);
	/* Signal handler'ı set et - CTRL+C için */
	setup_fork_signal(base, pipefd, &content);
	/* Heredoc content'ini oku */
	content = read_heredoc_input_child(delimiter, base, 0);
	if (content)
	{
		write(pipefd[1], content, ft_strlen(content));
		free(content);
	}
	/* Normal cleanup - signal handler değil */
	cleanup_child_resources(base);
	close(pipefd[1]);
	exit(0);
}

/* Parent process - değişiklik yok */
int	handle_parent_process(int pipefd[2], pid_t pid, t_base *base)
{
	int	status;

	if (pipefd[1] != -1)
	{
		close(pipefd[1]);
	}
	waitpid(pid, &status, 0);
	/* SIGINT ile child sonlandıysa */
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		base->exit_status = 130;
		return (0);
	}
	/* Normal exit code 130 kontrolü */
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		base->exit_status = 130;
		return (0);
	}
	return (1);
}

/* Read from pipe - değişiklik yok */
char	*read_from_pipe(int fd)
{
	char	*content;
	char	*temp;
	char	buffer[1024];
	ssize_t	bytes_read;

	content = malloc(1);
	if (!content)
		return (NULL);
	content[0] = '\0';
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	}
	return (content);
}

/* Main fork function - değişiklik yok */
char	*run_heredoc_child_to_file(char *delimiter, t_base *base,
		t_heredoc_info *info_node)
{
	int pipefd[2];
	pid_t pid;
	char *content;

	(void)info_node;
	if (pipe(pipefd) == -1)
		return (NULL);

	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}
	if (pid == 0)
	{
		if (handle_child_process(pipefd, delimiter, base))
			return (NULL);
	}
	if (!handle_parent_process(pipefd, pid, base))
		return (NULL);

	content = read_from_pipe(pipefd[0]);
	close(pipefd[0]);
	return (content);
}