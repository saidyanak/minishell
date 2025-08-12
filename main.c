/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/08/12 11:49:53 by syanak           ###   ########.fr       */
=======
/*   Updated: 2025/08/12 12:39:34 by syanak           ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_tokens_and_heredocs(t_base *base)
{
	if (base->token)
	{
		cleanup_heredoc_files(base);
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
	heredoc_static_flag(-1);
}

int	handle_heredoc_processing(t_base *base)
{
	heredoc_static_flag(-1);
	process_all_heredocs(base);
<<<<<<< HEAD
	// Ctrl+C kontrolü
=======
>>>>>>> main
	if (*heredoc_static_flag(0) == 1 || base->exit_status == 130)
	{
		base->exit_status = 130;
		cleanup_tokens_and_heredocs(base);
		return (0);
	}
	if (base->exit_status != 0)
	{
		cleanup_tokens_and_heredocs(base);
		return (0);
	}
	return (1);
}

<<<<<<< HEAD
int	check_heredoc(t_token *token)
{
	t_token	*head;

	head = token;
	while (head)
	{
		if (head->type == TOKEN_HEREDOC)
		{
			return (1);
		}
		head = head->next;
	}
	return (0);
}

=======
>>>>>>> main
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
<<<<<<< HEAD
	tokenize_input(input, base);
	if (!base->token)
	{
		cleanup_tokens_and_heredocs(base);
		return ;
	}
	if (check_heredoc(base->token))
		handle_heredoc_processing(base);
	expand_tokens(base);
	execute_command(base);
	cleanup_tokens_and_heredocs(base);
=======
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
>>>>>>> main
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
			process_input(input, base);
		free(input);
	}
}

void	init_base_struct(t_base *base, char **env)
{
	base->token = NULL;
	base->exit_status = 0;
	base->heredocs = NULL;
	base->heredoc_count = 0;
	base->env = init_env(env);
	heredoc_static_flag(-1);
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
	init_base_struct(&base, env);
	update_shlvl(&base);
	setup_interactive_signals();
	run_shell_loop(&base);
	restore_signals();
<<<<<<< HEAD
=======

	// Final cleanup
>>>>>>> main
	cleanup_all(&base);
	clear_history();
	return (base.exit_status);
}