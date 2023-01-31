#include "serial.h"

#include <ft_prepro/color.h>
#include <ft_printf.h>
#include <libunit.h>
#include <termios.h>

#include <stdbool.h>

// clang-format off

#define BAUDX_condition(VALUE) \
	serial_decode_baudrate(PP_CAT(B, VALUE)) == VALUE

TEST_SECTION(baudrate_decode, extract_name, BAUDX_condition,
	(9600),
	(19200),
	(38400),
	(57600),
	(115200),
	(230400),
	(460800),
	(921600)
);

#define BAUDE_condition(VALUE) \
	serial_encode_baudrate(VALUE) == PP_CAT(B, VALUE)

TEST_SECTION(baudrate_encode, extract_name, BAUDE_condition,
	(9600),
	(19200),
	(38400),
	(57600),
	(115200),
	(230400),
	(460800),
	(921600)
);

// clang-format on

static void print_end(int* _)
{
	ft_printf("} %s// Section baudrate%s\n\n",
	          COLOR(BLUE), COLOR(NORMAL));
}

int test_baudrate()
{
    int __attribute__((cleanup(print_end))) a =
        ft_printf("%sEntering section baudrate%s {\n\n",
                  COLOR(BOLD, BLUE), COLOR(NORMAL));

    return test_baudrate_decode()
	    || test_baudrate_encode();
}
