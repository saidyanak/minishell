/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 17:11:25 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <unistd.h>

t_token	**split_commands(t_token *token, int cmd_count)
{
	t_token	**commands;
	t_token	*current;
	int		cmd_index;

	commands = malloc(sizeof(t_token *) * (cmd_count + 1));
	if (!commands)
		return (NULL);
	current = token;
	cmd_index = 0;
	commands[cmd_index++] = current;
	while (current && cmd_index < cmd_count)
	{
		if (current->type == TOKEN_PIPE)
		{
			// Token chain'i koparmıyoruz, sadece başlangıç pointer'larını kaydediyoruz
			if (current->next)
				commands[cmd_index++] = current->next;
		}
		current = current->next;
	}
	commands[cmd_index] = NULL;
	return (commands);
}

void	cleanup_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		i++;
	}
	free(pipes);
}

// Token chain'i restore etmek için koparmadan önce backup al
void	restore_token_chain(t_token **commands, int cmd_count)
{
	int		i;
	t_token	*current;

	i = 0;
	while (i < cmd_count - 1)
	{
		current = commands[i];
		// Her komutun sonuna kadar git
		while (current && current->next == NULL)
			current = current->next;
		// Bir sonraki komuttan önce pipe olmalı, restore et
		if (current && i + 1 < cmd_count)
		{
			// Pipe token'ı bul ve bağlantıyı restore et
			// Bu kısım daha karmaşık, alternatif çözüm daha iyi olabilir
		}
		i++;
	}
}
