/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 12:09:55 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_input(char *input, t_base *base)
{
	if (!input || !*input)
		return ;
	add_history(input);
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
	tokenize_input(input, base);
	if (!preprocess_heredocs(base))
	{
		printf("minishell: heredoc processing failed\n");
		return ;
	}
	expand_tokens(base);
	execute_command(base);
}

static void	run_shell_loop(t_base *base)
{
	char	*input;

	while (1)
	{
		input = readline("gameofshell$ ");
		base->exit_status = check_signal_status(base->exit_status);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			process_input(input, base);
			if (base->token)
			{
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
	base.heredocs = NULL;
	base.heredoc_count = 0;
	base.env = init_env(env);
	update_shlvl(&base);
	setup_interactive_signals();
	run_shell_loop(&base);
	restore_signals();
	cleanup_heredocs(&base);
	cleanup_all(&base);
	clear_history();
	return (base.exit_status);
}
