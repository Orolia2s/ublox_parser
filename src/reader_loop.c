#include "ublox_reader.h"

#include <stddef.h> // uint8_t

bool ublox_reader_loop(ublox_reader_t* reader)
{
	Array            message = ArrayNew(uint8_t);
	ublox_callback_t callback;

	while (ublox_next_message(reader->input, &message))
	{
		array_foreach(ublox_callback_t, &reader->callbacks, &callback)
		{
			callback((ublox_message_t*)array_first(&message));
		}
	}
	return true;
}
