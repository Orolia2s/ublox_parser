#include "serial.h"

#include <log.h>

#include <stdbool.h>
#include <unistd.h>

ssize_t serial_single_read(serial_port_t* port)
{
	t_deque* queue = &port->buffer;
	size_t   room  = ftq_capacity(queue) - ftq_size(queue);
	log_trace("%s(%i, %zu)", __PRETTY_FUNCTION__, port->file_descriptor, room);
	uint8_t  buffer[room];
	ssize_t  result = read(port->file_descriptor, buffer, room);

	if (result > 0)
		ftq_push_back(queue, buffer, result);
	return result;
}

/**
 * If possible, accumulate @p n bytes before returning
 * @return a boolean
 *  - `true` if n bytes were successfully accumulated
 *  - `false` otherwise
 */
bool serial_accumulate(serial_port_t* port, size_t n)
{
	// log_trace("%s(%i, %zu)", __PRETTY_FUNCTION__, port->file_descriptor, n);
	while (ftq_size(&port->buffer) < n)
		if (serial_single_read(port) <= 0)
			return false;
	return true;
}
