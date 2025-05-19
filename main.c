/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:12:06 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/16 14:33:12 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <stdio.h>

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
		tokenize_input(input, my_base);
		//execute_command(args, &my_env);
	    free(input);
	}
	return (0);
}
