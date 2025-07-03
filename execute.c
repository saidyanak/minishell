/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/03 17:22:45 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Forward declarations
static void	free_argv(char **argv);

int	check_build_in(char *input)
{
	return (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0
		|| ft_strcmp(input, "env") == 0 || ft_strcmp(input, "pwd") == 0
		|| ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0
		|| ft_strcmp(input, "export") == 0 || ft_strcmp(input, "list") == 0);
}
int execute_external_command(t_token *current_prompt, t_base *base)
{
	printf("Executing: %s\n", current_prompt->content);
	//PATH'le komut bul
	char **paths;
	paths = ft_split(getenv(""));
	
	return (0);
}


int single_execute_command(t_token *token, t_base *base)
{
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(token->content))
			ft_build_in(token, base);
		else
		{
			execute_external_command(token, base);
		}
	}
}

void	execute_command(t_base *base)
{
	t_token	*current_promt;

	current_promt = base->token;
	if (is_single_execute(current_promt))
		single_execute_command(current_promt, base);
	else
		multiple_execute_command(current_promt, base);

}
