#include "serial.h"
#include "file_input_stream.h"

void serial_close(serial_port_t* port)
{
	file_close(&port->file);
}
