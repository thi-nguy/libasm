#include <stddef.h>
#ifndef LIBASM_H
#define LIBASM_H



size_t ft_strlen(const char *s);
char *ft_strcpy(char *restrict dst, const char *restrict src);
int ft_strcmp(const char *s1, const char *s2);
size_t ft_write(int fd, const void *buf, size_t count);
size_t ft_read(int fd, const void *buf, size_t count);
char *ft_strdup(const char *s);

#endif