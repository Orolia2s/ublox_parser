#include <blackmagic/color.h> // COLOR
#include <blackmagic/for.h>   // FOR EACH
#include <blackmagic/join.h>  // JOIN

#include <stdio.h> // printf

/*
 * Certain tests are meaningless if another test fails,
 * the order important : the first failing test should be
 * fixed before investigation other failling tests, as
 * there might be a dependency.
 */
#define TEST_LIST \
	serial_options, messages_size, checksum, parse

#define DECLARE(NAME) \
    int JOIN(test, NAME)(void);

#define CALL(SUCCESS, TOTAL, NAME) \
    SUCCESS += ! JOIN(test, NAME)(); \
    TOTAL++;

FOR(EACH(TEST_LIST), DECLARE)

int main()
{
	unsigned success = 0;
	unsigned total   = 0;

	FOR(EACH(TEST_LIST), CALL, success, total)
	printf("%sResult : %u / %u functions%s\n",
	       success == total ? COLOR(GREEN) : COLOR(RED),
	       success, total, COLOR(NORMAL));
	return (success != total);
}
