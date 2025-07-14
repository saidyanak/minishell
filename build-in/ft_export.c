/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:24:14 by syanak            #+#    #+#             */
/*   Updated: 2025/07/10 18:21:06 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

char	*parse_token(t_token *token)
{
	char	*str;
	char	*equal_pos;

	if (!token || !token->content)
		return (NULL);
	str = token->content;
	equal_pos = ft_strchr(str, '=');
	if (equal_pos)
		return (ft_substr(str, 0, equal_pos - str));
	return (ft_strdup(str));
}

char	*parse_value(t_token *token)
{
	char	*str;
	char	*equal_pos;

	if (!token || !token->content)
		return (NULL);
	str = token->content;
	equal_pos = ft_strchr(str, '=');
	if (equal_pos)
		return (ft_strdup(equal_pos + 1));
	return (NULL);
}

int	is_valid_char(char c, int is_first)
{
	if (is_first)
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0] || !is_valid_char(name[0], 1))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!is_valid_char(name[i], 0))
			return (0);
		i++;
	}
	return (1);
}

int	has_equal_sign(char *str)
{
	if (!str)
		return (0);
	return (ft_strchr(str, '=') != NULL);
}

t_env	*find_env_var(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*create_env_string(char *key, char *value)
{
	char	*env_str;
	char	*temp;

	if (!key)
		return (NULL);
	if (!value)
		return (ft_strdup(key));
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	env_str = ft_strjoin(temp, value);
	free(temp);
	return (env_str);
}

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
	new_node = create_new_node_gc(env_str, base->gc);
	if (new_node)
	{
		new_node->exported = has_equal_sign(token->content) ? 1 : 0;
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
		free(env_var->value);
		env_var->value = value ? value : ft_strdup("");
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

void	add_or_update(t_token *token, t_base *base)
{
	char	*key;
	t_env	*existing;

	key = parse_token(token);
	if (!key)
		return ;
	if (!validate_and_error(key, token->content))
	{
		free(key);
		return ;
	}
	existing = find_env_var(base->env, key);
	if (existing)
		update_env_var(existing, token);
	else
		add_env_var(token, base);
	free(key);
}

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

t_env	**create_env_array(t_env *env, int count)
{
	t_env	**array;
	int		i;

	array = malloc(sizeof(t_env *) * count);
	if (!array)
		return (NULL);
	i = 0;
	while (env && i < count)
	{
		array[i] = env;
		env = env->next;
		i++;
	}
	return (array);
}

int	ascii_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

void	sort_env_array(t_env **array, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ascii_strcmp(array[i]->key, array[j]->key) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}
int	has_equal_sign_in_original(t_env *env)
{
	return (env->exported == 1);
}

void	print_export_var(t_env *env)
{
	printf("declare -x %s", env->key);
	if (has_equal_sign_in_original(env))
		printf("=\"%s\"", env->value);
	printf("\n");
}

void	print_sorted_export(t_env *env)
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

void	print_export(t_base *base)
{
	print_sorted_export(base->env);
}

int	process_export_args(t_token *token, t_base *base)
{
	t_token	*current;

	current = token->next;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			add_or_update(current, base);
		current = current->next;
	}
	return (0);
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