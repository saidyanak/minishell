#include <unistd.h>
#include <stdio.h>

int main()
{
    char pwd[1024];

    printf("%s", getcwd(pwd ,sizeof(pwd)));
}