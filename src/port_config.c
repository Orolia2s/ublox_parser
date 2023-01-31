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
 * The baud rate can be set to other powers of 2 times of 9600, up to 921600
 */
bool ublox_port_config(serial_port_t* port)
{
	serial_print_config(port);
	cfsetspeed(&port->options.termios, B115200);
	serial_make_raw(port);
	serial_print_config(port);
	return true;
}
