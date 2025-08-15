/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:52:07 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/15 16:23:36 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_shlvl(t_base *base)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_value;

	shlvl_str = get_value(base->env, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	else
		shlvl = 0;
	shlvl++;
	new_value = ft_itoa(shlvl);
	set_env_value(&(base->env), "SHLVL", new_value);
	free(new_value);
}

void	set_underscore_variable(t_base *base, t_token *token)
{
	t_token	*last_token;
	char	*last_arg;

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

int	allocate_key_value(t_env *node, char *env, char *equal_sign)
{
	int	key_len;

	if (!equal_sign)
	{
		node->key = ft_strdup(env);
		node->value = initialize_empty_content_safe();
	}
	else
	{
		key_len = equal_sign - env;
		node->key = malloc(key_len + 1);
		if (node->key)
		{
			ft_strlcpy(node->key, env, key_len + 1);
			node->key[key_len] = '\0';
		}
		node->value = ft_strdup(equal_sign + 1);
	}
	if (!(node->key && node->value))
		return (0);
	return (1);
}

void	cleanup_node(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}
