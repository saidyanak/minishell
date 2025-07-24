#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    int pipefd[2];
    close(1);  // stdout'u kapatalım, ama örnek için bu önemli değil
    close(2);  // stderr kapalıysa perror görünmez!

    // Simüle edelim: 1000 tane pipe açıp sistem limitini aşalım
    for (int i = 0; i < 2000000; i++)
        pipe(pipefd); // Bir yerden sonra başarısız olacak

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return(-1);
    }
    return 0;
}