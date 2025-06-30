/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:38:03 by syanak            #+#    #+#             */
/*   Updated: 2025/06/30 12:11:12 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "minishell.h"
#include <stdio.h>



char	*find_target_path(t_token *current_promt, t_env *env)
{
	t_token	*promt;
	char	*pwd;

	if (current_promt->next != NULL)
		promt = current_promt->next;
	if (access(promt->content, W_OK) == -1)
	{
		printf("bash: cd: ls: Permission denied");
		return (NULL);
	}
	else
	{
		getcwd();
	}
}

t_base	ft_cd(t_token *current_promt, t_base base)
{
	char	*target_path;

	// ilk once gitmek istediğimiz yeri görmeden once birden fazla argüman girilmiş mi onu görelim.
	if (current_promt->next && current_promt->next->next)
	{
		printf("Said: cd: too many arguments");
		base.exit_status = 1;
		return (base);
	}
	// ardından target location u bulucaz
	target_path = find_target_path(current_promt, base.env);
	// peşine de target a girme işlemi yapıcaz
	// bunları yaparken oldpwd yi yani eski konumu tutmamız lazım ki
	// -paremetresi girilirse eski konuma gidilsin
	// bu eski konum meselesi tabiki bir "" içerisinde yada '' içerisinde gelirse onu bir dosya olarak algılayacağız.
	// kısacası işimiz kolay bunları getcwd access chdir gibi unistd.h kütüphanesinin içerisindeki fonksyonlarla yapıcaz
	// burası ekstra access fonksyonu gönderilen flag e göre yetki sorgulaması yazma sorgulaması yada "dir" ise yine yetki sorgusu
	// yapabiliyoruz bu sayede farklı hata durumları verebiliriz.
}
*/
#include "minishell.h"

t_base	ft_cd(t_token *current_promt, t_base base)
{
	(void)current_promt;
	return (base);
}