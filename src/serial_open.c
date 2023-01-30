#include "serial.h"

#include <fcntl.h> // open
#include <log.h>
#include <termios.h> // tcgetattr

#include <errno.h>
#include <stdbool.h>
#include <string.h> // strerror
#include <unistd.h> // close

/**
 * Open the port in read-only.
 */
serial_port_t serial_open(const char* port_name)
{
	serial_port_t port = {.file_descriptor = -1, .opened = false, .got_options = false};

	log_trace("%s(%s)", __PRETTY_FUNCTION__, port_name);
	if ((port.file_descriptor = open(port_name, O_RDONLY | O_NOCTTY)) < 0)
	{
		log_error("Unable to open \"%s\": %s", port_name, strerror(errno));
		return port;
	}
	port.opened = true;
	serial_get_options(&port);
	return port;
}

/**
 * Ask the port for its configurations.
 */
bool serial_get_options(serial_port_t* port)
{
	log_trace("%s(%i)", __PRETTY_FUNCTION__, port->file_descriptor);
	if (tcgetattr(port->file_descriptor, &port->options.termios) != 0)
	{
		log_error("Unable to get the attributes of the terminal: %s", strerror(errno));
		return false;
	}
	port->got_options = true;
	return true;
}

/**
 * Release resources to the system
 *
 * In case of error when calling close, there is absolutely nothing interesting
 * that can be done beyond reporting it, so it is not propagated.
 */
void serial_close(serial_port_t* port)
{
	log_trace("%s(%i)", __PRETTY_FUNCTION__, port->file_descriptor);
	if (!port->opened)
	{
		log_error("Attempting to close a port that isn't open.");
		return;
	}
	if (close(port->file_descriptor) != 0)
	{
		log_error("Unable to close the port: %s", strerror(errno));
	}
	port->opened = false;
}
