/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 15:58:12 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	process_input(char *input, t_base *base)
{
	heredoc_static_flag(-1);
	if (!input || !*input)
		return ;
	add_history(input);
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	tokenize_input(input, base);
	if (!base->token)
	{
		cleanup_tokens_and_heredocs(base);
		return ;
	}
	if (check_heredoc(base->token))
	{
		if (!handle_heredoc_processing(base))
			return ;
	}
	expand_tokens(base);
	execute_command(base);
	cleanup_tokens_and_heredocs(base);
}

static void	run_shell_loop(t_base *base)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$");
		base->exit_status = check_signal_status(base->exit_status);
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
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
	base->data = NULL;
	base->env = init_env(env);
	heredoc_static_flag(-1);
}

int	main(int argc, char **argv, char **env)
{
	t_base	base;

	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("No such file or directory\n", 2);
		exit(127);
	}
	init_base_struct(&base, env);
	update_shlvl(&base);
	setup_interactive_signals();
	run_shell_loop(&base);
	restore_signals();
	cleanup_all(&base);
	clear_history();
	return (base.exit_status);
}
