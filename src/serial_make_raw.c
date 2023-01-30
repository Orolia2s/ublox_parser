#include "serial.h"

#include <log.h>
#include <termios.h>

#include <errno.h>
#include <stdbool.h>
#include <string.h> // strerror
#include <unistd.h>

/**
 * General configuration for non-canonical mode
 *
 * Sets the terminal to something like the "raw" mode of the old Version 7 terminal
 * driver: input is available character by character, echoing is disabled, and
 * all special processing of terminal input and output characters is disabled.
 */
bool serial_make_raw(int port_fd)
{
	serial_options_t options;

	if (tcgetattr(port_fd, &options.termios) != 0)
	{
		log_error("Unable to get the attributes of the terminal: %s", strerror(errno));
		return false;
	}

	// cfmakeraw(&options.termios);

	options.input.ignore_break          = false;
	options.input.signal_break          = false;
	options.input.mark_errors           = false;
	options.input.strip_8th_bit         = false;
	options.input.map_nl_to_cr          = false;
	options.input.ignore_cr             = false;
	options.input.map_cr_to_nl          = false;
	options.input.enable_start_stop_out = false;

	options.control.character_size = character_size_8;
	options.control.enable_parity  = false;

	if (tcsetattr(port_fd, TCSANOW, &options.termios) != 0)
	{
		log_error("Unable to set the attributes of the terminal: %s", strerror(errno));
		return false;
	}
	return true;
}
