#include "serial.h"

#include <log.h>
#include <termios.h>
#include <ft_prepro/tools.h>
#include <errno.h>
#include <stdint.h>
#include <string.h> // strerror
#include <unistd.h>

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
	cfmakeraw(&port->options.termios);
	/*
	port->options.input.ignore_break          = false;
	port->options.input.signal_break          = false;
	port->options.input.mark_errors           = false;
	port->options.input.strip_8th_bit         = false;
	port->options.input.map_nl_to_cr          = false;
	port->options.input.ignore_cr             = false;
	port->options.input.map_cr_to_nl          = false;
	port->options.input.enable_start_stop_out = false;

	port->options.control.character_size = character_size_8;
	port->options.control.enable_parity  = false;
	*/

	if (tcsetattr(port->file_descriptor, TCSANOW, &port->options.termios) != 0)
	{
		log_error("Unable to set the attributes of the terminal: %s", strerror(errno));
		return false;
	}
	port->got_options = false;
	return true;
}

#define possible_baudrates \
	0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, \
	19200, 38400, 57600, 115200, 230400, 460800, 500000, 576000, \
	921600, 1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, \
	4000000

#define TO_BPS(_, V) case PP_CAT(B, V): return V;

/*
 * Return  the baudrate in bits per second
 */
int64_t serial_extract_baudrate(uint32_t flag)
{
	switch (flag)
	{
		FOR(EACH(possible_baudrates), TO_BPS)
	}
	return -1;
}
