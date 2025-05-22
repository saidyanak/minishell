/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:12:06 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/22 20:25:23 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_base	my_base;
	char	**args;

	(void)argc;
	(void)argv;
	my_base.env = init_env(envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		tokenize_input(input, &my_base);
		while (my_base.token)
		{
			printf("TOKEN: [%s] | TYPE: %d\n", my_base.token->content,
				my_base.token->type);
			my_base.token = my_base.token->next;
		}
		// execute_command(args, &my_env);
		free(input);
		// execute_command(args, &my_env);
	}
	return (0);
}
