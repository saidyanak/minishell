/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:12:06 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/19 12:29:21 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*my_env;
	char	**args;

	(void)argc;
	(void)argv;
	my_env = init_env(envp);
	while (1)
	{
	    input = readline("minishell$ ");
	    if (!input)
	        break ;
		args = tokenize_input(input);
		if (!args[3])
			break;
		//execute_command(args, &my_env);
	    free(input);

	}
	return (0);
}
