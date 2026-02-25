#include <stdio.h>
#include <string.h>
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

	printf("\n");
	return (0);
}
