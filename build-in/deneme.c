#include "../libft/libft.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int	main(void)
{
	char pwd[1024];

	printf("%d\n", chdir("/home/syanak"));
	printf("%s", getcwd(pwd, sizeof(pwd)));
	// printf("%s\n", ft_strjoin(pwd, "/deneme.c"));
	// printf("%d", access(ft_strjoin(pwd, "/deneme.c"), W_OK));
}