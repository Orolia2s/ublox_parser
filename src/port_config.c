#include "ublox.h"

#include <fcntl.h>
#include <ft_printf.h>
#include <libft.h>
#include <termios.h>

#include <unistd.h>

/**
 * @return
 *   - the opened port file descriptor
 *   - `-1` on error
 */
int ublox_open_serial_port(const char* port_name)
{
	struct termios options;
	int            port;

	port = open(port_name, O_RDWR | O_NOCTTY);

	tcgetattr(port, &options);

	ft_print_memory(&options, sizeof(options));
	ft_dprintf(STDIN_FILENO, "%u\n", options.c_cflag);

	// Set baud rate
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	// Set parity and stop bits
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;

	// Set character size
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Set flow control
	options.c_cflag &= ~CRTSCTS;

	// Set timeouts
	options.c_cc[VMIN]  = 1;
	options.c_cc[VTIME] = 5;

	// Set the attributes
	tcsetattr(port, TCSANOW, &options);

	return port;
}
