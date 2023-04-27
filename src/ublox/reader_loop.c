#include "ublox_reader.h"

#include <stdlib.h> // free

bool ublox_reader_loop(ublox_reader_t* reader)
{
	ublox_message_t* message;

	while ((message = ublox_next_message(reader->input)))
	{
		void *iterator;

		iterator = ARRAY_ITERATOR(&reader->callbacks);
		while (ARRAY_HASNEXT(&reader->callbacks, iterator))
		{
			(*(ublox_callback_t*)iterator)(message);
		}
		free(message);
	}
	return true;
}
