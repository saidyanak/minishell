/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/10 18:00:52 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// String'in tamamen numeric olup olmadığını kontrol et
static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

// Token sayısını say
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

// Exit code'u normalize et (0-255 arası)
static int	normalize_exit_code(int exit_code)
{
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	return (exit_code);
}

// Argüman sayısını kontrol et
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

// Argümanın numeric olup olmadığını kontrol et
static void	check_numeric_argument(t_token *arg, t_base *base)
{
	if (!is_numeric(arg->content))
	{
		printf("minishell: exit: %s: numeric argument required\n", arg->content);
		ft_free_all_env(base->gc);
		exit(2);
	}
}

// Exit fonksiyonu
void	ft_exit(t_token *current_prompt, t_base *base)
{
	int		exit_code;
	t_token	*arg;

	printf("exit\n");
	arg = current_prompt->next;
	if (!arg)
	{
		ft_free_all_env(base->gc);
		exit(base->exit_status);
	}
	if (!check_argument_count(arg, base))
		return ;
	check_numeric_argument(arg, base);
	exit_code = normalize_exit_code(ft_atoi(arg->content));
	ft_free_all_env(base->gc);
	exit(exit_code);
}
