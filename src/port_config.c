#include "serial.h"
#include "ublox.h"

#include <fcntl.h> // open
#include <libft.h> // print_memory
#include <log.h>
#include <termios.h> // tcgetattr

#include <errno.h>
#include <string.h> // strerror

/**
 * Congigure a serial port to receive messages from the u-blox receiver.
 *
 * The UART1 port of the F9T receiver uses the following configuration by default:
 *  - Data bits: 8
 *  - Parity: None
 *  - Stop bits: 1
 *  - Baud rate: 38400
 *
 * The baud rate can be set to other values:
 *  - 9600
 *  - 19200
 *  - 38400
 *  - 57600
 *  - 115200
 *  - 230400
 *  - 460800
 *  - 921600
 *
 * @return
 *   - the opened port file descriptor
 *   - `-1` on error
 */
int ublox_open_serial_port(const char* port_name)
{
	int            port;

	log_trace("%s(%s)\n", __PRETTY_FUNCTION__, port_name);
	if ((port = open(port_name, O_RDONLY | O_NOCTTY)) < 0)
	{
		log_error("Unable to open \"%s\": %s\n", port_name, strerror(errno));
		return -1;
	}
	serial_print_config(port);
	serial_make_raw(port);
	serial_print_config(port);
	return port;
}
