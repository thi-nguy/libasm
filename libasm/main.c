/* ************************************************************************** */
/*                                                                            */
/*   main.c - Test suite for libasm                                           */
/*   Tests: ft_read, ft_write, ft_strcmp, ft_strdup, ft_strcpy, ft_strlen     */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "libasm.h"

/* ─── Color codes ─────────────────────────────────────────────────────────── */
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define CYAN    "\033[0;36m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

/* ─── Helper macros ───────────────────────────────────────────────────────── */
#define PASS(label) printf(GREEN "  [PASS]" RESET " %s\n", label)
#define FAIL(label) printf(RED   "  [FAIL]" RESET " %s\n", label)
#define SECTION(name) printf("\n" BOLD CYAN "══ %s ══\n" RESET, name)
#define CHECK(cond, label) do { if (cond) PASS(label); else FAIL(label); } while (0)

static int g_pass = 0;
static int g_fail = 0;

#define ASSERT(cond, label) \
    do { \
        if (cond) { PASS(label); g_pass++; } \
        else      { FAIL(label); g_fail++; } \
    } while (0)

/* ══════════════════════════════════════════════════════════════════════════ */
/*  ft_strlen                                                                 */
/* ══════════════════════════════════════════════════════════════════════════ */
static void test_strlen(void)
{
    SECTION("ft_strlen");

    /* Basic strings */
    ASSERT(ft_strlen("hello") == strlen("hello"),           "strlen: \"hello\"");
    ASSERT(ft_strlen("") == strlen(""),                     "strlen: empty string");
    ASSERT(ft_strlen("a") == strlen("a"),                   "strlen: single char");
    ASSERT(ft_strlen("Hello, World!") == strlen("Hello, World!"),
                                                            "strlen: sentence");
    /* String with spaces */
    ASSERT(ft_strlen("  spaces  ") == strlen("  spaces  "), "strlen: spaces");
    /* String with special chars */
    ASSERT(ft_strlen("abc\tdef\n") == strlen("abc\tdef\n"), "strlen: \\t \\n");
    /* Long string */
    char longstr[1024];
    memset(longstr, 'A', 1023);
    longstr[1023] = '\0';
    ASSERT(ft_strlen(longstr) == strlen(longstr),           "strlen: 1023-char string");
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  ft_strcpy                                                                 */
/* ══════════════════════════════════════════════════════════════════════════ */
static void test_strcpy(void)
{
    SECTION("ft_strcpy");

    char dst1[64], dst2[64];

    /* Basic copy */
    strcpy(dst2, "hello");
    ft_strcpy(dst1, "hello");
    ASSERT(strcmp(dst1, dst2) == 0, "strcpy: basic \"hello\"");

    /* Empty string */
    strcpy(dst2, "");
    ft_strcpy(dst1, "");
    ASSERT(strcmp(dst1, dst2) == 0, "strcpy: empty string");

    /* Single char */
    strcpy(dst2, "x");
    ft_strcpy(dst1, "x");
    ASSERT(strcmp(dst1, dst2) == 0, "strcpy: single char");

    /* Return value == dst */
    char *ret = ft_strcpy(dst1, "retval");
    ASSERT(ret == dst1, "strcpy: return value == dst pointer");

    /* String with special chars */
    strcpy(dst2, "line1\nline2\ttab");
    ft_strcpy(dst1, "line1\nline2\ttab");
    ASSERT(strcmp(dst1, dst2) == 0, "strcpy: \\n and \\t");

    /* Null terminator placed correctly */
    memset(dst1, 0xFF, sizeof(dst1));
    ft_strcpy(dst1, "end");
    ASSERT(dst1[3] == '\0', "strcpy: null terminator at correct position");
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  ft_strcmp                                                                 */
/* ══════════════════════════════════════════════════════════════════════════ */

/* Normalise to -1 / 0 / 1 so we compare sign, not raw value */
static int sign(int n) { return (n > 0) - (n < 0); }

static void test_strcmp(void)
{
    SECTION("ft_strcmp");

    /* Equal strings */
    ASSERT(sign(ft_strcmp("abc", "abc")) == sign(strcmp("abc", "abc")),
           "strcmp: equal strings \"abc\"");
    ASSERT(sign(ft_strcmp("", "")) == sign(strcmp("", "")),
           "strcmp: both empty");

    /* First < second */
    ASSERT(sign(ft_strcmp("abc", "abd")) == sign(strcmp("abc", "abd")),
           "strcmp: \"abc\" < \"abd\"");
    ASSERT(sign(ft_strcmp("", "a")) == sign(strcmp("", "a")),
           "strcmp: \"\" < \"a\"");
    ASSERT(sign(ft_strcmp("abc", "abcd")) == sign(strcmp("abc", "abcd")),
           "strcmp: prefix shorter");

    /* First > second */
    ASSERT(sign(ft_strcmp("abd", "abc")) == sign(strcmp("abd", "abc")),
           "strcmp: \"abd\" > \"abc\"");
    ASSERT(sign(ft_strcmp("a", "")) == sign(strcmp("a", "")),
           "strcmp: \"a\" > \"\"");
    ASSERT(sign(ft_strcmp("abcd", "abc")) == sign(strcmp("abcd", "abc")),
           "strcmp: prefix longer");

    /* Case sensitivity */
    ASSERT(sign(ft_strcmp("ABC", "abc")) == sign(strcmp("ABC", "abc")),
           "strcmp: case sensitive \"ABC\" vs \"abc\"");
    ASSERT(sign(ft_strcmp("abc", "ABC")) == sign(strcmp("abc", "ABC")),
           "strcmp: case sensitive \"abc\" vs \"ABC\"");

    /* High ASCII / unsigned comparison */
    ASSERT(sign(ft_strcmp("\x80", "\x01")) == sign(strcmp("\x80", "\x01")),
           "strcmp: high-ASCII unsigned comparison");
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  ft_strdup                                                                 */
/* ══════════════════════════════════════════════════════════════════════════ */
static void test_strdup(void)
{
    SECTION("ft_strdup");

    char *dup;

    /* Basic dup */
    dup = ft_strdup("hello");
    ASSERT(dup != NULL,                 "strdup: \"hello\" not NULL");
    ASSERT(strcmp(dup, "hello") == 0,   "strdup: \"hello\" content match");
    free(dup);

    /* Empty string */
    dup = ft_strdup("");
    ASSERT(dup != NULL,                 "strdup: \"\" not NULL");
    ASSERT(strcmp(dup, "") == 0,        "strdup: \"\" content match");
    free(dup);

    /* Pointer differs from original */
    const char *orig = "separate";
    dup = ft_strdup(orig);
    ASSERT(dup != orig,                 "strdup: new allocation (pointer differs)");
    ASSERT(strcmp(dup, orig) == 0,      "strdup: content matches original");
    free(dup);

    /* Long string */
    char longstr[1024];
    memset(longstr, 'Z', 1023);
    longstr[1023] = '\0';
    dup = ft_strdup(longstr);
    ASSERT(dup != NULL,                 "strdup: long string not NULL");
    ASSERT(strcmp(dup, longstr) == 0,   "strdup: long string content match");
    free(dup);

    /* String with special chars */
    dup = ft_strdup("line\nnewline\ttab");
    ASSERT(dup && strcmp(dup, "line\nnewline\ttab") == 0,
                                        "strdup: \\n and \\t");
    free(dup);
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  ft_write  (man 2 write)                                                   */
/* ══════════════════════════════════════════════════════════════════════════ */
static void test_write(void)
{
    SECTION("ft_write");

    /* ── stdout ── */
    printf("  [INFO] ft_write to stdout (fd=1): ");
    fflush(stdout);
    ssize_t ret = ft_write(1, "ft_write OK\n", 12);
    ASSERT(ret == 12, "write: return value == nbytes on stdout");

    /* ── write 0 bytes ── */
    ret = ft_write(1, "should not appear", 0);
    ASSERT(ret == 0, "write: 0 bytes returns 0");

    /* ── write to a temp file ── */
    int fd = open("/tmp/libasm_write_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ASSERT(fd >= 0, "write: open temp file");
    if (fd >= 0)
    {
        ret = ft_write(fd, "LIBASM_TEST\n", 12);
        ASSERT(ret == 12, "write: 12 bytes to file");
        close(fd);

        /* Verify content */
        fd = open("/tmp/libasm_write_test.txt", O_RDONLY);
        char buf[32] = {0};
        read(fd, buf, 31);
        close(fd);
        ASSERT(strcmp(buf, "LIBASM_TEST\n") == 0, "write: file content matches");
        unlink("/tmp/libasm_write_test.txt");
    }

    /* ── invalid fd → errno ── */
    errno = 0;
    ret = ft_write(-1, "bad fd", 6);
    ASSERT(ret == -1,       "write: invalid fd returns -1");
    ASSERT(errno == EBADF,  "write: invalid fd sets errno=EBADF");
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  ft_read  (man 2 read)                                                     */
/* ══════════════════════════════════════════════════════════════════════════ */
static void test_read(void)
{
    SECTION("ft_read");

    /* ── prepare a temp file ── */
    const char *path = "/tmp/libasm_read_test.txt";
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ASSERT(fd >= 0, "read: create temp file");
    if (fd < 0) return;
    write(fd, "HelloWorld", 10);
    close(fd);

    /* ── normal read ── */
    fd = open(path, O_RDONLY);
    char buf[32] = {0};
    ssize_t ret = ft_read(fd, buf, 5);
    ASSERT(ret == 5,                    "read: returns nbytes read (5)");
    ASSERT(memcmp(buf, "Hello", 5) == 0,"read: content matches first 5 bytes");

    /* ── partial / continuing read ── */
    memset(buf, 0, sizeof(buf));
    ret = ft_read(fd, buf, 5);
    ASSERT(ret == 5,                    "read: second read returns 5");
    ASSERT(memcmp(buf, "World", 5) == 0,"read: second read content matches");

    /* ── EOF ── */
    ret = ft_read(fd, buf, 10);
    ASSERT(ret == 0,                    "read: EOF returns 0");
    close(fd);

    /* ── read 0 bytes ── */
    fd = open(path, O_RDONLY);
    ret = ft_read(fd, buf, 0);
    ASSERT(ret == 0,                    "read: 0 bytes returns 0");
    close(fd);

    /* ── invalid fd → errno ── */
    errno = 0;
    ret = ft_read(-1, buf, 10);
    ASSERT(ret == -1,       "read: invalid fd returns -1");
    ASSERT(errno == EBADF,  "read: invalid fd sets errno=EBADF");

    unlink(path);
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  Summary                                                                   */
/* ══════════════════════════════════════════════════════════════════════════ */
static void summary(void)
{
    int total = g_pass + g_fail;
    printf("\n" BOLD "══ SUMMARY ══\n" RESET);
    printf(GREEN "  Passed : %d / %d\n" RESET, g_pass, total);
    if (g_fail)
        printf(RED "  Failed : %d / %d\n" RESET, g_fail, total);
    else
        printf(GREEN "  All tests passed!\n" RESET);
}

/* ══════════════════════════════════════════════════════════════════════════ */
/*  main                                                                      */
/* ══════════════════════════════════════════════════════════════════════════ */
int main(void)
{
    printf(BOLD "\n=== libasm Test Suite ===\n" RESET);

    test_strlen();
    test_strcpy();
    test_strcmp();
    test_strdup();
    test_write();
    test_read();

    summary();
	
    return (g_fail > 0 ? 1 : 0);
}