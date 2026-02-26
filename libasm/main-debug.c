#include <stdio.h>
#include <string.h>
#include "include/libasm.h"

int main(void)
{
    int ft;
    char *s1 = "abc";
    char *s2 = "abd";
    ft  = ft_strcmp(s1, s2);
    printf("%d\n", ft);
    return (0);
}