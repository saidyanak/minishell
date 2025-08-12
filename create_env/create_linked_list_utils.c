/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:52:07 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/12 14:58:29 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_null_check(t_env *env_lst)
{
	if (!env_lst)
	{
		printf("minishell: error initializing environment\n");
		exit(1);
	}
}

void	update_shlvl(t_base *base)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_value;

	shlvl_str = get_value(base->env, "SHLVL");
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	new_value = ft_itoa(shlvl);
	set_env_value(&(base->env), "SHLVL", new_value);
	free(new_value);
}

void	set_underscore_variable(t_base *base, t_token *token)
{
	t_token	*last_token;
	char	*last_arg;
	char	*heredoc;

	heredoc = "<<";
	if (!token || !base)
		return ;
	last_token = token;
	while (last_token->next && !is_redirection_token(last_token->next->type))
		last_token = last_token->next;
	if (last_token->content)
		last_arg = last_token->content;
	else
		last_arg = "";
	set_env_value(&(base->env), "_", last_arg);
}
