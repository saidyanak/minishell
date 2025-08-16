/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:13:20 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 17:46:52 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>

int	handle_heredoc_processing(t_base *base)
{
	heredoc_static_flag(-1);
	base->exit_status = 0;
	process_all_heredocs(base);
	if (*heredoc_static_flag(0) == 1 || base->exit_status == 130)
	{
		base->exit_status = 130;
		cleanup_tokens_and_heredocs(base);
		return (0);
	}
	if (base->exit_status != 0)
	{
		cleanup_tokens_and_heredocs(base);
		return (0);
	}
	return (1);
}

int	check_heredoc(t_token *token)
{
	t_token	*head;

	head = token;
	while (head)
	{
		if (head->type == TOKEN_HEREDOC)
		{
			return (1);
		}
		head = head->next;
	}
	return (0);
}

char	*check_return(char *line)
{
	if (line != NULL)
		return (ft_strdup(line));
	else
		return (ft_strdup(""));
	return (NULL);
}

char	*unique_name_return(char *temp, char *counter_str, char *temp_file)
{
	char	*unique_name;

	unique_name = NULL;
	unique_name = ft_strjoin(temp, counter_str);
	free(temp);
	free(counter_str);
	if (!unique_name)
		return (NULL);
	temp_file = ft_strjoin("/tmp/heredoc_", unique_name);
	free(unique_name);
	return (temp_file);
}

char	*heredoc_parent_process(char *content, int *pipefd, pid_t pid,
		t_base *base)
{
	content = read_heredoc_content(pipefd, pid, base);
	if (base->exit_status == 130)
	{
		rl_cleanup_after_signal();
		rl_reset_after_signal();
		rl_initialize();
		return (NULL);
	}
	return (content);
}
