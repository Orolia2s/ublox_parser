#include "serial.h"

#include <libunit.h>
#include <termios.h>

#include <stdbool.h>

// clang-format off

#define BAUD_condition(VALUE) \
	serial_extract_baudrate(PP_CAT(B, VALUE)) == VALUE

TEST_SECTION(baudrate, extract_name, BAUD_condition,
	(9600),
	(19200),
	(38400),
	(57600),
	(115200),
	(230400),
	(460800),
	(921600)
);
