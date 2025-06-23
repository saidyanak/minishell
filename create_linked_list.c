/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/19 13:03:05 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft/libft.h"

t_env *create_new_node(char *env)
{
    t_env   *new_node;
    char    *equal_sign;
    int     key_len;

    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return(NULL);
    equal_sign = ft_strchr(env, '=');
    if (!equal_sign)
    {
        free(new_node); // Bellek sızıntısını önle
        return (NULL);
    }
    key_len = equal_sign - env;
    new_node->key = ft_strndup(env, key_len);
    new_node->value = ft_strdup(equal_sign + 1);
    new_node->next = NULL;
    return (new_node);
}

void add_new_node(t_env **head, t_env *new_node)
{
    t_env   *tmp;
    
    if (!new_node) // Yeni düğüm NULL ise ekleme yapma
        return;
    
    if (!*head)
        *head = new_node;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
}

t_env *init_env(char **env)
{
    int     i;
    t_env   *env_list = NULL; // t_env* olarak değiştirildi
    t_env   *node;

    i = 0;
    while (env[i])
    {
        node = create_new_node(env[i]);
        if (node)
            add_new_node(&env_list, node);
        i++;
    }
    return (env_list);
}