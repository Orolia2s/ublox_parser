#include "ublox_reader.h"

#include <stdlib.h> // free

bool ublox_reader_loop(ublox_reader_t* reader)
{
	ublox_message_t* message;
	ublox_callback_t callback;

	while ((message = ublox_next_message(reader->input)))
	{
		array_foreach(ublox_callback_t, &reader->callbacks, &callback)
		{
			callback(message);
		}
		free(message);
	}
	return true;
}
