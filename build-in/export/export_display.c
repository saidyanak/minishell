/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:14:43 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 12:12:31 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
