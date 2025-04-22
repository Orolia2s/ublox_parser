#include "ublox_reader.h"

#include <o2s/timer.h> // o2s_timer_setup_process

#include <stddef.h> // uint8_t

bool ublox_reader_loop(ublox_reader_t* reader)
{
	Array            message = ArrayNew(uint8_t);
	ublox_callback_t callback;

	if (!o2s_timer_setup_process(file_default_signal_handler))
		return false;

	O2sTimer timer = o2s_timer_create();

	if (!timer.created)
		return false;
	while (ublox_next_message_with_timeout(reader->input, &message, &timer, reader->timeout_ms))
	{
		array_foreach(ublox_callback_t, &reader->callbacks, &callback)
		{
			callback((ublox_message_t*)array_first(&message));
		}
	}
	return true;
}
