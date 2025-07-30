/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/30 16:23:12 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	process_input(char *input, t_base *base)
{
	if (!input || !*input)
		return ;
	add_history(input);
	// Önceki token'ları temizle
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	tokenize_input(input, base);
	// print_tokens(base->token);
}

static void	run_shell_loop(t_base *base)
{
	char	*input;

	while (1)
	{
		input = readline("gameofshell$ ");
		if (!input) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			process_input(input, base);
			if (base->token)
			{			
				expand_tokens(base);
				execute_command(base);
				free_tokens(base->token);
				base->token = NULL;
			}
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_base	base;

	(void)argv;
	if (argc > 1)
	{
		printf("One more argument error\n");
		exit(1);
	}
	base.token = NULL;
	base.exit_status = 0;
	base.env = init_env(env);
	setup_signals();
	run_shell_loop(&base);
	cleanup_all(&base);
	clear_history();
	return (base.exit_status);
}
