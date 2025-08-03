/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:23:55 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/03 13:34:13 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	check_build_in(char *input)
{
	if (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0)
		return (1);
	if (ft_strcmp(input, "env") == 0 || ft_strcmp(input, "pwd") == 0)
		return (1);
	if (ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0)
		return (1);
	if (ft_strcmp(input, "export") == 0 || ft_strcmp(input, "list") == 0)
		return (1);
	return (0);
}

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
