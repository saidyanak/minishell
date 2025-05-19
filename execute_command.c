/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:44:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/12 14:47:24 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Amac Bize gelen args double pointerın içerisindeki komutları çalıştırmak 
	3 ana başlıkta ele alabiliriz
		1. Build-in komutlar 7 tane.
		2. execve ile calıstırılacaklar (usr/bin/*) ls pip vs..
		3. << >> gibi operatörler.
*/
void	execute_command(char	**args, t_env	*my_env)// my_env program çalışınca ortam değişkenlerinin bulunduğu singel linkedlist yapı.
{
	
}