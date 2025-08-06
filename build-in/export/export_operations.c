/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:15:03 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/06 10:32:36 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_env_var(t_token *token, t_base *base)
{
	char	*key;
	char	*value;
	char	*env_str;
	t_env	*new_node;

	key = parse_token(token);
	value = parse_value(token);
	if (!key)
	{
		free(value);
		return ;
	}
	env_str = create_env_string(key, value);
	new_node = create_new_node(env_str);
	if (new_node)
	{
		if (has_equal_sign(token->content))
			new_node->exported = 1;
		else
			new_node->exported = 0;
		add_new_node(&(base->env), new_node);
	}
	free(key);
	free(value);
	free(env_str);
}

void	update_env_var(t_env *env_var, t_token *token)
{
	char	*value;

	if (has_equal_sign(token->content))
	{
		value = parse_value(token);
		if (env_var->value)
			free(env_var->value);
		if (value)
			env_var->value = value;
		else
			env_var->value = initialize_empty_content_safe();
		env_var->exported = 1;
	}
}

int	validate_and_error(char *key, char *content)
{
	if (!is_valid_identifier(key))
	{
		printf("minishell: export: `%s': not a valid identifier\n", content);
		return (0);
	}
	return (1);
}

int	add_or_update_env(t_token *token, t_base *base)
{
	char	*key;
	t_env	*existing;

	key = parse_token(token);
	if (!key)
		return (1);
	if (!validate_and_error(key, token->content))
	{
		free(key);
		return (1);
	}
	existing = find_env_var(base->env, key);
	if (existing)
		update_env_var(existing, token);
	else
		add_env_var(token, base);
	free(key);
	return (0);
}
