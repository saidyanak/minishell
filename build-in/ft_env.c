/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:45:26 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/01 15:16:22 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int   ft_env(t_base base)
{
    t_env   *head;
    head = base.env;
    while (head->next != NULL)
    {
        printf("%s", head->key);
        printf("=");
        printf("%s", head->value);
        printf("\n");
        head = head->next;
        base.exit_status = 0; 
    }
    return(base.exit_status);
}