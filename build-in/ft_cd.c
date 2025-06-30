/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:38:03 by syanak            #+#    #+#             */
/*   Updated: 2025/06/30 11:10:57 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

t_base	ft_cd(t_token *current_promt, t_base base)
{
	char	*target_path;

	if (current_promt->next && current_promt->next->next
		&& current_promt->next->content == '|'
		&& current_promt->next->next->content != '|')
	{
		printf("Said: cd: too many arguments");
		base.exit_status = 1;
		return (base);
	}
	// ilk once gitmek istediğimiz yeri görmeden once birden fazla argüman girilmiş mi onu görelim.
	// ardından target location u bulucaz
	// peşine de target a girme işlemi yapıcaz
	// bunları yaparken oldpwd yi yani eski konumu tutmamız lazım ki
	-paremetresi girilirse eski konuma gidilsin
	// bu eski konum meselesi tabiki bir "" içerisinde yada '' içerisinde gelirse onu bir dosya olarak algılayacağız.
	// kısacası işimiz kolay bunları getcwd access chdir gibi unistd.h kütüphanesinin içerisindeki fonksyonlarla yapıcaz
	// burası ekstra access fonksyonu gönderilen flag e göre yetki sorgulaması yazma sorgulaması yada "dir" ise yine yetki sorgusu
	// yapabiliyoruz bu sayede farklı hata durumları verebiliriz.
}
