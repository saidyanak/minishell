/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/22 09:39:14 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Basit heredoc - pipe kullanarak
int	simple_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0) // Child - input oku ve pipe'a yaz
	{
		close(pipefd[0]); // Read end'i kapat
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				if (line)
					free(line);
				break ;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		exit(0);
	}
	else if (pid > 0) // Parent
	{
		close(pipefd[1]);              // Write end'i kapat
		dup2(pipefd[0], STDIN_FILENO); // Pipe'ı stdin'e yönlendir
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
		return (0);
	}
	else
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
}

// Execute_multiple.c'de kullanım için wrapper
int	setup_simple_heredoc(char *delimiter)
{
	return (simple_heredoc(delimiter));
}