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
bool ublox_port_config(serial_port_t* port, int64_t baudrate)
{
	serial_ensure_options(port);
	port->options.input_speed  = serial_encode_baudrate(baudrate);
	port->options.output_speed = serial_encode_baudrate(baudrate);
	port->options.control_characters.minimum = ublox_smallest_message_size;
	port->options.control_characters.timeout = 15; // 1.5 second
	serial_make_raw(port);
	return true;
}
