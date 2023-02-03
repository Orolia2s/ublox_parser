#include "serial.h"
#include "ublox.h"

#include <fcntl.h> // open
#include <libft.h> // print_memory
#include <log.h>
#include <termios.h> // tcgetattr

#include <errno.h>
#include <string.h> // strerror

bool _ublox_port_set_speed(serial_port_t* port, int64_t baudrate)
{
	speed_t speed = serial_encode_baudrate(baudrate);

	if (speed == 0)
		return false;
	port->options.input_speed  = speed;
	port->options.output_speed = speed;
	port->options.input._dont_modify_speed = false;
	port->options.control._speed_4lsb = speed;
	port->options.control._speed_is_extra = speed >> 12;
	return true;
}

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
 *
 * @param baudrate: line speed in bits per second
 */
bool ublox_port_config(serial_port_t* port, int64_t baudrate)
{
	serial_ensure_options(port);
	if (!_ublox_port_set_speed(port, baudrate))
		return false;
	port->options.control_characters.minimum = ublox_smallest_message_size;
	port->options.control_characters.timeout = 15; // 1.5 second
	return serial_make_raw(port);
}
