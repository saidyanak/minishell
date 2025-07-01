/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 11:59:25 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/01 19:36:00 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>


char *get_env_value(t_base base, char *key)
{
    t_env *current;
    
    if (!base.env || !key)
        return (NULL);
    current = base.env;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

static int is_get_env_value(t_base base)
{
    write(1, get_env_value(base, "PWD"), ft_strlen(get_env_value(base, "PWD")));
    write(1, "\n",1 );
    base.exit_status = 0;
    return (base.exit_status);
}

int ft_pwd(t_base base)
{
    char *cwd;
    
    cwd = getcwd(NULL, 0);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        write(1,cwd, ft_strlen(cwd));
        write(1, "\n", 1);
        free(cwd);
        base.exit_status = 0;
        return (base.exit_status);
    }
    else
    {
        if (get_env_value(base, "PWD") != NULL)
        {
            return (is_get_env_value(base));
        }
        else
        {
            perror("getcwd() error");
            base.exit_status = 1;
            return (base.exit_status);
        }
    }
}
