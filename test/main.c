#include <ft_prepro/color.h>
#include <ft_prepro/tools.h>
#include <ft_printf.h>

/*
 * Certain tests are meaningless if another test fails,
 * the order important : the first failing test should be
 * fixed before investigation other failling tests, as
 * there might be a dependency.
 */
#define TEST_LIST \
	serial_options, baudrate, port_config, \
	messages_size, checksum

#define DECLARE(IGNORED, NAME) \
    int MERGE(test, NAME)(void);

#define CALL(SUCCESS, TOTAL, NAME) \
    SUCCESS += ! MERGE(test, NAME)(); \
    TOTAL++;

FOR(EACH(TEST_LIST), DECLARE, IGNORED)

int main()
{
    unsigned success = 0;
    unsigned total = 0;

    FOR(EACH(TEST_LIST), CALL, success, total)
    ft_printf("%sResult : %u / %u functions%s\n",
              success == total ? COLOR(GREEN) : COLOR(RED),
              success, total, COLOR(NORMAL));
    return (success != total);
}
