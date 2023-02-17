#include "serial.h"

#include <log.h>
#include <termios.h> // tcgetattr

#include <errno.h>
#include <stdbool.h>
#include <string.h> // strerror

/**
 * Ask the port for its configurations.
 */
bool serial_get_options(serial_port_t* port)
{
	log_trace("%s(%i)", __FUNCTION__, port->file.descriptor);
	if (tcgetattr(port->file.descriptor, &port->options.termios) != 0)
	{
		log_error("Unable to get the attributes of the terminal: %s", strerror(errno));
		return false;
	}
	port->got_options = true;
	return true;
}
