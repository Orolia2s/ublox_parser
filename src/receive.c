#include "serial.h"
#include "ublox.h"
#include "ublox_enums.h"

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
	t_deque* queue = &port->file.buffer;
	t_array  result[] = {NEW_ARRAY(uint8_t)};

sync:
	while (serial_accumulate(port, sizeof(ublox_sync_chars)
	                         + ublox_smallest_message_size)
	       && *(uint8_t*)ftq_first(queue) != ublox_sync_chars[0])
	{
		FTQ_POP_FRONT_ONE(queue, NULL);
	}
	if (ftq_is_empty(queue) || *(uint8_t*)ftq_first(queue) != ublox_sync_chars[0])
		return NULL;
	FTQ_POP_FRONT_ONE(queue, NULL);
	if (*(uint8_t*)ftq_first(queue) != ublox_sync_chars[1])
		goto sync;
	FTQ_POP_FRONT_ONE(queue, NULL);
	if (!is_valid_ublox_class(*(uint8_t*)ftq_first(queue)))
	{
		log_trace("Discaring message of unknown class %#.2hhx",
		          *(uint8_t*)ftq_first(queue));
		goto sync;
	}
	ftq_pop_front_into_array(queue, result, sizeof(struct ublox_header));

	ublox_message_t** message = (ublox_message_t**)&result->data;
	if (!serial_accumulate(port, message[0]->length + sizeof(struct ublox_footer)))
	{
		log_trace(
			"Unable to read completely a message of class %s, discarding header",
			ublox_class_strings[message[0]->class]);
		goto fail;
	}
	ftq_pop_front_into_array(queue, result, message[0]->length);
	fta_trim(result);
	{
		ublox_checksum_t computed = ublox_compute_checksum(result->data, result->size);
		ublox_checksum_t expected;
		ftq_pop_front(queue, &expected, sizeof(expected));
		if (*(uint16_t*)&computed != *(uint16_t*)&expected)
		{
			log_warn("Discarding message of type %s with invalid checksum",
			         ublox_class_strings[message[0]->class]);
			goto sync;
		}
	}
	return result->data;
fail:
	fta_clear(result);
	return NULL;
}
