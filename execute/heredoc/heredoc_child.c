/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/07 17:49:10 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	heredoc_signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	should_expand_heredoc(char *delimiter)
{
	int	i;

	i = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
			return (0);
		i++;
	}
	return (1);
}

char	*remove_quotes_from_delimiter(char *delimiter)
{
	int		i;
	int		j;
	char	*result;

	result = malloc(ft_strlen(delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] != '\'' && delimiter[i] != '"')
			result[j++] = delimiter[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*read_heredoc_input_child(char *delimiter, t_base *base, int expand)
{
	char	*line;
	char	*content;
	char	*clean_delimiter;

	(void)expand;
	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	if (!clean_delimiter)
		return (NULL);
	content = NULL;
	while (1)
	{
		if (!process_readline_input(&line, clean_delimiter, &content, base))
			break ;
		if (!content)
		{
			free(clean_delimiter);
			return (NULL);
		}
	}
	free(clean_delimiter);
	return (finalize_heredoc_content(content));
}
