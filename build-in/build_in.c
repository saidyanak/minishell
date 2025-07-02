/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:08:48 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/02 14:12:46 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_list(t_token *token, t_base *base)
{
	t_env	*head;

	head = base->env;
	print_tokens(token);
	printf("---------ENV-----------\n");
	while (head)
	{
		printf("key:%s, value:%s, exported:%d\n", head->key, head->value,
			head->exported);
		head = head->next;
	}
}

// echo export unset exit cd pwd
void	ft_build_in(t_token *current_prompt, t_base *base)
{
	if (ft_strcmp(current_prompt->content, "echo") == 0) // aocak
		ft_echo(current_prompt, *base);
	else if (ft_strcmp(current_prompt->content, "cd") == 0) // said
		ft_cd(current_prompt, base);
	else if (ft_strcmp(current_prompt->content, "pwd") == 0) // aocak
		ft_pwd(*base);
	else if (ft_strcmp(current_prompt->content, "env") == 0) // aocak
		ft_env(base);
	else if (ft_strcmp(current_prompt->content, "export") == 0) // said
		ft_export(current_prompt, base);
	else if (ft_strcmp(current_prompt->content, "unset") == 0) // said
		ft_unset(current_prompt, base);
	else if (ft_strcmp(current_prompt->content, "list") == 0)
		ft_list(current_prompt, base);
	// else if (ft_strcmp(current_prompt->content, "exit") == 0) //said
	//  ft_exit(current_prompt, base);
}
