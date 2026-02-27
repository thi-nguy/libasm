#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "include/libasm.h"
 #define COLOR_GREEN "\033[0;32m"
#define COLOR_RED   "\033[0;31m"
#define COLOR_RESET "\033[0m"

static void	run_test(const char *label, const char *input)
{
	size_t	expected;
	size_t	got;
	int		pass;

	/* NULL guard: strlen(NULL) is UB, chỉ test ft_strlen */
	if (input == NULL)
	{
		got = ft_strlen(NULL);
		printf("%-35s │ ft_strlen(NULL) = %zu\n", label, got);
		return ;
	}

	expected = strlen(input);
	got      = ft_strlen(input);
	pass     = (expected == got);

	printf("%-35s │ strlen=%-5zu │ ft_strlen=%-5zu │ %s%s%s\n",
		label,
		expected,
		got,
		pass ? COLOR_GREEN : COLOR_RED,
		pass ? "PASS" : "FAIL",
		COLOR_RESET);
}

static void	run_strcpy_test(const char *label, const char *src)
{
	char	buf_ref[512];
	char	buf_ft[512];
	char	*ret_ref;
	char	*ret_ft;
	int		content_ok;
	int		return_ok;
	int		pass;

	/* Dùng strcpy làm chuẩn */
	ret_ref = strcpy(buf_ref, src);
	ret_ft  = ft_strcpy(buf_ft, src);

	/* Kiểm tra nội dung chuỗi sau copy */
	content_ok = (strcmp(buf_ref, buf_ft) == 0);

	/* Kiểm tra return value phải trỏ về dst */
	return_ok  = (ret_ft == buf_ft);

	pass = content_ok && return_ok;

	printf("%-35s │ ref=\"%-10s\" │ ft=\"%-10s\" │ %s%s%s\n",
		label,
		buf_ref,
		buf_ft,
		pass ? COLOR_GREEN : COLOR_RED,
		pass ? "PASS" : "FAIL",
		COLOR_RESET);

	/* In chi tiết khi fail */
	if (!content_ok)
		printf("  %s[FAIL] Content mismatch%s\n", COLOR_RED, COLOR_RESET);
	if (!return_ok)
		printf("  %s[FAIL] Return value != dst%s\n", COLOR_RED, COLOR_RESET);
	(void)ret_ref;
}

static void	run_strcmp_test(const char *label, const char *s1, const char *s2)
{
	int	ref;
	int	ft;
	int	pass;

	ref = strcmp(s1, s2);
	ft  = ft_strcmp(s1, s2);

	/* strcmp chỉ đảm bảo dấu của kết quả */
	pass = ((ref == 0 && ft == 0)
		|| (ref < 0 && ft < 0)
		|| (ref > 0 && ft > 0));

	printf("%-35s │ ref=%-4d │ ft=%-4d │ %s%s%s\n",
		label,
		ref,
		ft,
		pass ? COLOR_GREEN : COLOR_RED,
		pass ? "PASS" : "FAIL",
		COLOR_RESET);
}

static int g_pass = 0;
static int g_fail = 0;

static void print_result(const char *test_name, int passed)
{
    if (passed)
    {
        printf("  [PASS] %s\n", test_name);
        g_pass++;
    }
    else
    {
        printf("  [FAIL] %s\n", test_name);
        g_fail++;
    }
}

/*
** So sánh:
**   - return value của ft_write vs write
**   - errno sau khi gọi (chỉ khi cả hai đều thất bại)
*/
static void compare(const char *test_name,
                    ssize_t ret_ft, int errno_ft,
                    ssize_t ret_sys, int errno_sys)
{
    int ret_ok   = (ret_ft == ret_sys);
    int errno_ok = 1;

    /* Chỉ kiểm errno khi cả hai báo lỗi */
    if (ret_sys < 0 && ret_ft < 0)
        errno_ok = (errno_ft == errno_sys);

    if (ret_ok && errno_ok)
        print_result(test_name, 1);
    else
    {
        print_result(test_name, 0);
        if (!ret_ok)
            printf("         ret   ft_write=%zd  write=%zd\n", ret_ft, ret_sys);
        if (!errno_ok)
            printf("         errno ft_write=%d (%s)  write=%d (%s)\n",
                   errno_ft, strerror(errno_ft),
                   errno_sys, strerror(errno_sys));
    }
}

/* ------------------------------------------------------------------ */
/*  Tests                                                               */
/* ------------------------------------------------------------------ */

/* 1. Ghi chuỗi bình thường ra stdout */
static void test_normal_stdout(void)
{
    printf("\n[1] Ghi chuỗi ra stdout:\n");

    const char *msg = "ft_write: Hello, World!\n";
    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(STDOUT_FILENO, msg, strlen(msg));
    e_ft  = errno;

    errno = 0;
    r_sys = write(STDOUT_FILENO, msg, strlen(msg));
    e_sys = errno;

    compare("write to stdout (return value)", r_ft, e_ft, r_sys, e_sys);
}

/* 2. Ghi 0 byte (count = 0) */
static void test_zero_bytes(void)
{
    printf("\n[2] Ghi 0 byte (count = 0):\n");

    const char *buf = "anything";
    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(STDOUT_FILENO, buf, 0);
    e_ft  = errno;

    errno = 0;
    r_sys = write(STDOUT_FILENO, buf, 0);
    e_sys = errno;

    compare("write 0 bytes", r_ft, e_ft, r_sys, e_sys);
}

/* 3. Ghi vào file descriptor hợp lệ (file tạm) */
static void test_write_to_file(void)
{
    printf("\n[3] Ghi vào file tạm:\n");

    const char *path_ft  = "/tmp/ft_write_test_ft.txt";
    const char *path_sys = "/tmp/ft_write_test_sys.txt";
    const char *data     = "test data 123\n";

    int fd_ft  = open(path_ft,  O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd_sys = open(path_sys, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd_ft < 0 || fd_sys < 0)
    {
        printf("  [SKIP] Không mở được file tạm\n");
        if (fd_ft  >= 0) close(fd_ft);
        if (fd_sys >= 0) close(fd_sys);
        return;
    }

    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(fd_ft, data, strlen(data));
    e_ft  = errno;

    errno = 0;
    r_sys = write(fd_sys, data, strlen(data));
    e_sys = errno;

    close(fd_ft);
    close(fd_sys);

    compare("write to file (return value)", r_ft, e_ft, r_sys, e_sys);

    /* Kiểm tra nội dung file giống nhau */
    char buf_ft[64]  = {0};
    char buf_sys[64] = {0};

    fd_ft  = open(path_ft,  O_RDONLY);
    fd_sys = open(path_sys, O_RDONLY);
    read(fd_ft,  buf_ft,  sizeof(buf_ft)  - 1);
    read(fd_sys, buf_sys, sizeof(buf_sys) - 1);
    close(fd_ft);
    close(fd_sys);

    print_result("write to file (content match)", strcmp(buf_ft, buf_sys) == 0);

    unlink(path_ft);
    unlink(path_sys);
}

/* 4. Ghi vào stderr */
static void test_write_stderr(void)
{
    printf("\n[4] Ghi vào stderr:\n");

    const char *msg = "stderr test\n";
    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(STDERR_FILENO, msg, strlen(msg));
    e_ft  = errno;

    errno = 0;
    r_sys = write(STDERR_FILENO, msg, strlen(msg));
    e_sys = errno;

    compare("write to stderr", r_ft, e_ft, r_sys, e_sys);
}

/* 5. fd không hợp lệ — kỳ vọng: trả về -1, errno = EBADF */
static void test_invalid_fd(void)
{
    printf("\n[5] fd không hợp lệ (fd = -1):\n");

    const char *buf = "data";
    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(-1, buf, strlen(buf));
    e_ft  = errno;

    errno = 0;
    r_sys = write(-1, buf, strlen(buf));
    e_sys = errno;

    compare("invalid fd returns -1",      r_ft  == -1,    0, r_sys  == -1,    0);
    compare("invalid fd errno == EBADF",  e_ft == EBADF,  0, e_sys == EBADF,  0);
    compare("ft_write errno == write errno", r_ft, e_ft, r_sys, e_sys);
}

/* 6. fd là read-only — kỳ vọng: trả về -1, errno = EBADF */
static void test_readonly_fd(void)
{
    printf("\n[6] fd read-only:\n");

    const char *path = "/tmp/ft_write_readonly.txt";
    /* Tạo file trước */
    int tmp = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tmp >= 0) { write(tmp, "x", 1); close(tmp); }

    int fd_ft  = open(path, O_RDONLY);
    int fd_sys = open(path, O_RDONLY);

    if (fd_ft < 0 || fd_sys < 0)
    {
        printf("  [SKIP] Không mở được file read-only\n");
        if (fd_ft  >= 0) close(fd_ft);
        if (fd_sys >= 0) close(fd_sys);
        unlink(path);
        return;
    }

    const char *buf = "data";
    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(fd_ft,  buf, strlen(buf));
    e_ft  = errno;

    errno = 0;
    r_sys = write(fd_sys, buf, strlen(buf));
    e_sys = errno;

    close(fd_ft);
    close(fd_sys);
    unlink(path);

    compare("read-only fd", r_ft, e_ft, r_sys, e_sys);
}

/* 7. buf = NULL — kỳ vọng: trả về -1, errno = EFAULT */
// static void test_null_buf(void)
// {
//     printf("\n[7] buf = NULL:\n");

//     ssize_t r_ft, r_sys;
//     int e_ft, e_sys;

//     errno = 0;
//     r_ft  = ft_write(STDOUT_FILENO, NULL, 1);
//     e_ft  = errno;

//     errno = 0;
//     r_sys = write(STDOUT_FILENO, NULL, 1);
//     e_sys = errno;

//     compare("NULL buf", r_ft, e_ft, r_sys, e_sys);
// }

/* 8. fd đã đóng — kỳ vọng: EBADF */
static void test_closed_fd(void)
{
    printf("\n[8] fd đã đóng:\n");

    const char *path = "/tmp/ft_write_closed.txt";

    int fd_ft  = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd_sys = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    /* Lưu fd rồi đóng ngay */
    int raw_ft  = fd_ft;
    int raw_sys = fd_sys;
    close(fd_ft);
    close(fd_sys);
    unlink(path);

    const char *buf = "data";
    ssize_t r_ft, r_sys;
    int e_ft, e_sys;

    errno = 0;
    r_ft  = ft_write(raw_ft, buf, strlen(buf));
    e_ft  = errno;

    errno = 0;
    r_sys = write(raw_sys, buf, strlen(buf));
    e_sys = errno;

    compare("closed fd", r_ft, e_ft, r_sys, e_sys);
}

int	main(void)
{
	printf("\n=== ft_strlen vs strlen ===\n\n");
	printf("%-35s │ %-12s │ %-15s │ %s\n",
		"Test case", "strlen", "ft_strlen", "Result");
	printf("%.35s─┼─%.12s─┼─%.15s─┼─%.6s\n",
		"───────────────────────────────────",
		"────────────", "───────────────", "──────");

	/* Chuỗi thông thường */
	run_test("Empty string \"\"",           "");
	run_test("Single char \"a\"",           "a");
	run_test("\"Hello, World!\"",           "Hello, World!");
	run_test("\"42 School\"",               "42 School");
	run_test("Spaces only \"   \"",         "   ");

	/* Chuỗi dài */
	run_test("100 'x' chars",
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	/* Ký tự đặc biệt */
	run_test("Newline/tab \"\\n\\t\\r\"",   "\n\t\r");
	run_test("Non-ASCII \"é à ü\"",         "é à ü");
	run_test("Numbers \"1234567890\"",       "1234567890");
	run_test("Null byte early \"ab\\0cd\"",  "ab"); /* strlen dừng ở \\0 */

	/* NULL pointer — chỉ ft_strlen, bỏ comment nếu bạn xử lý NULL */
	/* run_test("NULL pointer",              NULL); */

	/* ─── ft_strcpy vs strcpy ─────────────────────────────────────────────── */
	printf("\n=== ft_strcpy vs strcpy ===\n\n");
	printf("%-35s │ %-16s │ %-16s │ %s\n",
		"Test case", "ref (strcpy)", "ft (ft_strcpy)", "Result");
	printf("%.35s─┼─%.16s─┼─%.16s─┼─%.6s\n",
		"───────────────────────────────────",
		"────────────────", "────────────────", "──────");

	/* Chuỗi thông thường */
	run_strcpy_test("Empty string \"\"",         "");
	run_strcpy_test("Single char \"a\"",         "a");
	run_strcpy_test("\"Hello, World!\"",         "Hello, World!");
	run_strcpy_test("\"42 School\"",             "42 School");
	run_strcpy_test("Spaces only \"   \"",       "   ");

	/* Ký tự đặc biệt */
	run_strcpy_test("Newline/tab \"\\n\\t\\r\"", "\n\t\r");
	run_strcpy_test("Numbers \"1234567890\"",     "1234567890");
	run_strcpy_test("Non-ASCII \"é à ü\"",       "é à ü");

	/* Chuỗi dài */
	run_strcpy_test("50 'A' chars",
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

	/* Overwrite: copy ngắn hơn vào buffer đã có nội dung dài hơn */
	{
		char	buf_ref[64];
		char	buf_ft[64];

		strcpy(buf_ref,    "LongStringInBuffer");
		strcpy(buf_ft,     "LongStringInBuffer");
		strcpy(buf_ref,    "Short");
		ft_strcpy(buf_ft,  "Short");

		int pass = (strcmp(buf_ref, buf_ft) == 0);
		printf("%-35s │ ref=\"%-10s\" │ ft=\"%-10s\" │ %s%s%s\n",
			"Overwrite longer with shorter",
			buf_ref, buf_ft,
			pass ? COLOR_GREEN : COLOR_RED,
			pass ? "PASS" : "FAIL",
			COLOR_RESET);
	}

	/* ─── ft_strcmp vs strcmp ─────────────────────────────────────────────── */
printf("\n=== ft_strcmp vs strcmp ===\n\n");
printf("%-35s │ %-8s │ %-8s │ %s\n",
	"Test case", "strcmp", "ft_strcmp", "Result");
printf("%.35s─┼─%.8s─┼─%.8s─┼─%.6s\n",
	"───────────────────────────────────",
	"────────", "────────", "──────");

/* Chuỗi giống nhau */
run_strcmp_test("Both empty",                "", "");
run_strcmp_test("Same string",               "Hello", "Hello");
run_strcmp_test("Same with spaces",          "42 School", "42 School");

/* Khác nhau */
run_strcmp_test("First < second",            "abc", "abd");
run_strcmp_test("First > second",            "abd", "abc");
run_strcmp_test("Prefix vs longer",          "abc", "abcd");
run_strcmp_test("Longer vs prefix",          "abcd", "abc");

/* Ký tự đặc biệt */
run_strcmp_test("Numbers",                   "123", "124");
run_strcmp_test("Special chars",             "\n\t", "\n");
run_strcmp_test("Non-ASCII",                 "é", "ê");

/* Khác ở ký tự cuối */
run_strcmp_test("Diff at last char",         "testA", "testB");

/* Empty vs non-empty */
run_strcmp_test("Empty vs non-empty",        "", "a");
run_strcmp_test("Non-empty vs empty",        "a", "");

	printf("\n");
 printf("=== ft_write test suite ===\n");
    printf("Kiểm tra ft_write vs write (man 2 write)\n");

    test_normal_stdout();
    test_zero_bytes();
    test_write_to_file();
    test_write_stderr();
    test_invalid_fd();
    test_readonly_fd();
    // test_null_buf();
    test_closed_fd();

    printf("\n===========================\n");
    printf("Kết quả: %d passed, %d failed\n", g_pass, g_fail);
    printf("===========================\n");

    return (g_fail > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
