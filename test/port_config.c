#include "ublox.h"

#include <ft_prepro/color.h>
#include <ft_printf.h>
#include <libft.h>
#include <libunit.h>
#include <termios.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

void _serial_make_raw(serial_port_t*);
void _ublox_port_set_speed(serial_port_t*, int64_t);

static int test_make_raw_empty()
{
	serial_port_t  mine;
	serial_port_t  std;

	memset(&std.options, 0, sizeof(std.options));
	memset(&mine.options, 0, sizeof(mine.options));

	cfmakeraw(&std.options.termios);
	_serial_make_raw(&mine);
	return memcmp(&std.options, &mine.options, sizeof(std.options));
}

static int test_make_raw_full()
{
	serial_port_t  mine;
	serial_port_t  std;

	memset(&std.options, 0xff, sizeof(std.options));
	memset(&mine.options, 0xff, sizeof(mine.options));

	cfmakeraw(&std.options.termios);
	_serial_make_raw(&mine);
	return memcmp(&std.options, &mine.options, sizeof(std.options));
}

int test_make_raw()
{
	t_tests tests __attribute__((cleanup(fta_clear))) = NEW_ARRAY(t_test);

	load_test(&tests, "make_raw empty", test_make_raw_empty);
	load_test(&tests, "make_raw full", test_make_raw_full);
	return launch_tests("port_config", &tests);
}

static int _test_set_speed(int64_t speed_bps, uint8_t initial)
{
	serial_port_t  mine;
	serial_port_t  std;

	memset(&std.options, initial, sizeof(std.options));
	memset(&mine.options, initial, sizeof(mine.options));

	cfsetspeed(&std.options.termios, serial_encode_baudrate(speed_bps));
	_ublox_port_set_speed(&mine, speed_bps);
	//serial_print_config(&std);
	return !memcmp(&std.options, &mine.options, sizeof(std.options));
}


#define SPEED_empty_condition(SPEED_BPS) \
	_test_set_speed(SPEED_BPS, 0)

TEST_SECTION(set_speed_empty, extract_name, SPEED_empty_condition,
	(9600),
	(19200),
	(38400),
	(57600),
	(115200),
	(230400),
	(460800),
	(921600)
);


#define SPEED_full_condition(SPEED_BPS) \
	_test_set_speed(SPEED_BPS, 0xff)

TEST_SECTION(set_speed_full, extract_name, SPEED_full_condition,
	(9600),
	(19200),
	(38400),
	(57600),
	(115200),
	(230400),
	(460800),
	(921600)
);

int test_port_config()
{
	TEST_GROUP("port config");

	return test_make_raw()
		|| test_set_speed_empty()
		|| test_set_speed_full();
}
