#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv, char **env)
{
	// execve ile ls komutunu çalıştır
	char *args[] = {"ls", "-al", NULL};

	execve("/usr/bin/ls", args, env);

	// Eğer execve başarısız olursa burası çalışır
	perror("execve failed");
	return (1);
}