#include "serial.h"
#include "ublox.h"

#include <fcntl.h>
#include <ft_printf.h>
#include <libft.h>
#include <termios.h>

#include <unistd.h>

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
	struct termios options;
	int            port;

	if ((port = open(port_name, O_RDWR | O_NOCTTY)) < 0)
		return -1;

	tcgetattr(port, &options);

	//ft_print_memory(&options, sizeof(options));
	ft_print_memory(&options.c_iflag, sizeof(options.c_iflag));
	ft_print_memory(&options.c_cflag, sizeof(options.c_cflag));

	return port;
}
