/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/03 14:34:29 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	count_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

static int	normalize_exit_code(int exit_code)
{
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	return (exit_code);
}

static int	check_argument_count(t_token *arg, t_base *base)
{
	if (count_tokens(arg) > 1)
	{
		printf("minishell: exit: too many arguments\n");
		base->exit_status = 1;
		return (0);
	}
	return (1);
}

static void	check_numeric_argument(t_token *arg, t_base *base)
{
	if (!is_numeric(arg->content))
	{
		printf("minishell: exit: %s: numeric argument required\n",
			arg->content);
		cleanup_all(base);
		exit(2);
	}
}

void	ft_exit(t_token *current_prompt, t_base *base)
{
	int		exit_code;
	t_token	*arg;

	printf("exit\n");
	arg = current_prompt->next;
	if (!arg)
	{
		cleanup_all(base);
		exit(base->exit_status);
	}
	if (!check_argument_count(arg, base))
		return ;
	check_numeric_argument(arg, base);
	exit_code = normalize_exit_code(ft_atoi(arg->content));
	cleanup_all(base);
	exit(exit_code);
}
