/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:14:55 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 10:40:50 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	sort_and_print_env(t_env *env)
{
	t_env	**array;
	int		count;
	int		i;

	count = count_env_vars(env);
	if (count == 0)
		return ;
	array = create_env_array(env, count);
	if (!array)
		return ;
	sort_env_array(array, count);
	i = 0;
	while (i < count)
	{
		print_export_var(array[i]);
		i++;
	}
	free(array);
}

int	process_export_args(t_token *token, t_base *base)
{
	t_token	*current;

	current = token->next;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			add_or_update_env(current, base);
		current = current->next;
	}
	return (0);
}

void	print_export(t_base *base)
{
	t_env	**array;
	int		count;
	int		i;

	count = count_env_vars(base->env);
	if (count == 0)
		return ;
	array = create_env_array(base->env, count);
	if (!array)
		return ;
	sort_env_array(array, count);
	i = 0;
	while (i < count)
	{
		print_export_var(array[i]);
		i++;
	}
	free(array);
}

t_base	*ft_export(t_token *token, t_base *base)
{
	if (!token || !base)
		return (base);
	if (!token->next)
	{
		print_export(base);
		base->exit_status = 0;
		return (base);
	}
	base->exit_status = process_export_args(token, base);
	return (base);
}
