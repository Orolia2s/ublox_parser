#include "serial.h"
#include "ublox.h"

#include <ft_array.h>
#include <ft_printf.h>
#include <libft.h>
#include <log.h>

#include <sys/types.h> // ssize_t

#include <stdlib.h>

static void ftq_pop_front_into_array(t_deque* queue, t_array* array, size_t count)
{
	fta_reserve(array, count);
	ftq_pop_front(queue, ARRAY_END(array), count);
	array->size += count;
}

ublox_message_t* ublox_next_message(serial_port_t* port)
{
	t_deque* queue = &port->buffer;
	t_array  result[] = {NEW_ARRAY(uint8_t)};

	//log_trace("%s(%i)", __PRETTY_FUNCTION__, port->file_descriptor);

sync:
	while (serial_accumulate(port, sizeof(ublox_sync_chars)
	                         + ublox_smallest_message_size)
	       && *(uint8_t*)ftq_first(queue) != ublox_sync_chars[0])
	{
		//log_trace("Discarding: %#hhx\n", *(uint8_t*)ftq_first(queue));
		FTQ_POP_FRONT_ONE(queue, NULL);
	}
	if (*(uint8_t*)ftq_first(queue) != ublox_sync_chars[0])
		return NULL;
	//log_trace("Sync0: %#hhx\n", *(uint8_t*)ftq_first(queue));
	FTQ_POP_FRONT_ONE(queue, NULL);
	if (*(uint8_t*)ftq_first(queue) != ublox_sync_chars[1])
		goto sync;
	//log_trace("Sync1: %#hhx\n", *(uint8_t*)ftq_first(queue));
	FTQ_POP_FRONT_ONE(queue, NULL);
	log_trace("Class ? %#.2hhx", *(uint8_t*)ftq_first(queue));
	if (!is_valid_ublox_class(*(uint8_t*)ftq_first(queue)))
		goto sync;

	log_trace("Found the beginning of a %s message",
	          ublox_class_strings[*(uint8_t*)ftq_first(queue)]);
	ftq_pop_front_into_array(queue, result, sizeof(struct ublox_header));

	ublox_message_t* message = result->data; // Dangerous alias, careful
	log_debug("Potential message of class %s and type %#hhx, of length %hu",
	          ublox_class_strings[message->class],
	          message->type, message->length);
	if (!serial_accumulate(port, message->length + sizeof(struct ublox_footer)))
		goto fail;
	ftq_pop_front_into_array(queue, result, message->length + sizeof(struct ublox_footer));
	fta_trim(result);
	return result->data;
fail:
	fta_clear(result);
	return NULL;
}
