/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/07/22 10:17:10 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_export_assignment(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i] == '=')
		return (1);
	return (0);
}

static char	*extract_variable_name(char *str)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = ft_substr(str, 0, i);
	return (name);
}

static char	*extract_variable_value(char *str)
{
	int		i;
	char	*value;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	value = ft_strdup(str + i);
	return (value);
}

static void	add_or_update_env_var(t_base *base, char *name, char *value)
{
	t_env	*current;
	t_env	*new_env;

	current = base->env;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup(name);
	new_env->value = ft_strdup(value);
	new_env->exported = 1;
	new_env->next = base->env;
	base->env = new_env;
}

static void	handle_export_assignment_token(t_token *token, t_base *base)
{
	char	*var_name;
	char	*var_value;
	char	*processed_value;

	if (!is_export_assignment(token->content))
		return ;
	var_name = extract_variable_name(token->content);
	var_value = extract_variable_value(token->content);
	if (!var_name || !var_value)
	{
		if (var_name)
			free(var_name);
		if (var_value)
			free(var_value);
		return ;
	}
	processed_value = process_mixed_quotes(var_value, base);
	if (processed_value)
	{
		add_or_update_env_var(base, var_name, processed_value);
		free(processed_value);
	}
	free(var_name);
	free(var_value);
}

static int	is_export_command(t_token *token)
{
	if (!token || !token->content)
		return (0);
	if (ft_strcmp(token->content, "export") == 0)
		return (1);
	return (0);
}

static void	mark_token_for_deletion(t_token *token)
{
	if (token && token->content)
	{
		free(token->content);
		token->content = NULL;
	}
}

void	handle_export_assignments(t_base *base)
{
	t_token	*current;
	int		in_export_context;

	if (!base || !base->token)
		return ;
	current = base->token;
	in_export_context = 0;
	while (current)
	{
		if (is_export_command(current))
		{
			in_export_context = 1;
			current = current->next;
			continue ;
		}
		if (in_export_context && is_export_assignment(current->content))
		{
			handle_export_assignment_token(current, base);
			mark_token_for_deletion(current);
		}
		else
			in_export_context = 0;
		current = current->next;
	}
}

static void	remove_null_tokens(t_base *base)
{
	t_token	*current;
	t_token	*prev;

	while (base->token && (base->token->content == NULL
			|| ft_strlen(base->token->content) == 0))
		base->token = base->token->next;
	if (!base->token)
		return ;
	current = base->token;
	prev = NULL;
	while (current)
	{
		if (current->content == NULL || ft_strlen(current->content) == 0)
		{
			if (prev)
				prev->next = current->next;
			current = current->next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void	process_export_and_expand(t_base *base)
{
	handle_export_assignments(base);
	remove_null_tokens(base);
	expand_tokens(base);
}