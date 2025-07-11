/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/10 18:04:20 by yuocak           ###   ########.fr       */
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
		ft_free_all_tmp(base->gc);
		base->token = NULL;
	}
	tokenize_input(input, base);
	// Debug için token'ları yazdır (geliştirme aşamasında)
	print_tokens(base->token);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_base	base;
	t_gc	gc;

	(void)argc;
	(void)argv;
	gc_init(&gc);
	base.token = NULL;
	base.gc = &gc;
	base.env = init_env(env, base.gc);
	base.exit_status = 0;
	setup_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (*input) // Sadece boş olmayan input'ları işle
		{
			process_input(input, &base);
			if (base.token) // Tokenize başarılı olduysa devam et
			{
				expand_tokens(&base);
				execute_command(&base);
			}
		}
		free(input);
	}
	ft_free_all_env(base.gc);
	return (base.exit_status);
}
