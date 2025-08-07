/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/07 15:45:15 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_input(char *input, t_base *base)
{
	if (!input || !*input)
		return ;
	add_history(input);
	// Önceki kaynakları tamamen temizle
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
	// Yeni komut için tokenize et
	tokenize_input(input, base);
	if (!preprocess_heredocs(base))
	{
		printf("minishell: heredoc processing failed\n");
		// Error durumunda da tam cleanup
		cleanup_heredocs(base);
		if (base->token)
		{
			free_tokens(base->token);
			base->token = NULL;
		}
		return ;
	}
	// Expand ve execute
	expand_tokens(base);
	execute_command(base);
	// ZORUNLU: Her komut sonrası tam temizlik
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	// Bu çok önemli - heredoc'lar kesinlikle temizlenmeli
	cleanup_heredocs(base);
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
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_base base;

	(void)argv;
	if (argc > 1)
	{
		printf("One more argument error\n");
		exit(1);
	}

	// Base struct'ını initialize et
	base.token = NULL;
	base.exit_status = 0;
	base.heredocs = NULL;
	base.heredoc_count = 0;
	base.env = init_env(env);

	update_shlvl(&base);
	setup_interactive_signals();
	run_shell_loop(&base);
	restore_signals();

	// Final cleanup
	cleanup_all(&base);
	clear_history();
	return (base.exit_status);
}