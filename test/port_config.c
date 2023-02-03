#include "ublox.h"

#include <libft.h>
#include <libunit.h>
#include <termios.h>

#include <stdbool.h>
#include <string.h>
#include <unistd.h>

void _serial_make_raw(serial_port_t*);

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

int test_port_config()
{
	t_tests tests __attribute__((cleanup(fta_clear))) = NEW_ARRAY(t_test);

	load_test(&tests, "make_raw empty", test_make_raw_empty);
	load_test(&tests, "make_raw full", test_make_raw_full);
	return launch_tests("port_config", &tests);
}
