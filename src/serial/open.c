#include "file_input_stream.h"
#include "serial.h"

#include <fcntl.h>

/**
 * Open the port in read-only, and allocate the buffer.
 */
serial_port_t serial_open(const char* port_name)
{
	serial_port_t port = {.file = file_open(port_name, O_RDONLY | O_NOCTTY)};
	return port;
}
