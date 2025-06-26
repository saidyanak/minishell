/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:08:48 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/26 18:43:58 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
// echo export unset exit cd pwd 
void ft_build_in(t_token *current_prompt, t_base base)
{
    if (ft_strcmp(current_prompt->content, "echo") == 1)//aocak
        ft_echo(current_prompt, base); 
<<<<<<< HEAD
    else if (ft_strcmp(current_prompt->content, "cd") == 0) //said
       ft_cd(current_prompt, base);
=======
    else if (ft_strcmp(current_prompt->content, "cd") == 1) //said
        ft_cd(current_prompt, base);
>>>>>>> 91e2958c349ef3260dd87d1ec87cf0a6b57e00b6
    //else if (ft_strcmp(current_prompt->content, "pwd") == 0)// aocak
      //  ft_pwd(current_prompt, base);
    //else if (ft_strcmp(current_prompt->content, "unset") == 0) //said
      //  ft_unset(current_prompt, base);
    //else if (ft_strcmp(current_prompt->content, "export") == 0) //said
      //  ft_export(current_prompt, base);
    //else if (ft_strcmp(current_prompt->content, "exit") == 0) //said
      //  ft_exit(current_prompt, base);
    //else if (ft_strcmp(current_prompt->content, "env") == 0)//aocak
     //   ft_env(current_prompt, base);
}
