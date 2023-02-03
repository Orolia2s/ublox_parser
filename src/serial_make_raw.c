#include "serial.h"

#include <log.h>
#include <termios.h>

#include <errno.h>
#include <string.h> // strerror
#include <unistd.h>

void _serial_make_raw(serial_port_t* port)
{
	port->options.input.ignore_break          = false;
	port->options.input.signal_break          = false;
	port->options.input.mark_errors           = false;
	port->options.input.strip_8th_bit         = false;
	port->options.input.map_nl_to_cr          = false;
	port->options.input.ignore_cr             = false;
	port->options.input.map_cr_to_nl          = false;
	port->options.input.enable_start_stop_out = false;

	port->options.output.enable_processing = false;

	port->options.control.character_size = character_size_8;
	port->options.control.enable_parity  = false;

	port->options.local.enable_signals    = false;
	port->options.local.canonical         = false;
	port->options.local.echo              = false;
	port->options.local.echo_nl           = false;
	port->options.local.enable_processing = false;

	/* Not mentionned in documentation */
	port->options.control_characters.timeout = 0;
	port->options.control_characters.minimum = 1;
}

/**
 * General configuration for non-canonical mode
 *
 * Sets the terminal to something like the "raw" mode of the old Version 7 terminal
 * driver: input is available character by character, echoing is disabled, and
 * all special processing of terminal input and output characters is disabled.
 */
bool serial_make_raw(serial_port_t* port)
{
	serial_ensure_options(port);
	_serial_make_raw(port);
	if (tcsetattr(port->file_descriptor, TCSANOW, &port->options.termios) != 0)
	{
		log_error("Unable to set the attributes of the terminal: %s", strerror(errno));
		return false;
	}
	port->got_options = false;
	return true;
}
