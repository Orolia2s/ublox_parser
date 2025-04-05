#include "ublox.h"
#include "ublox_enums.h"

#include <o2s/array.h>
#include <o2s/log.h>

#include <stddef.h> // size_t

enum parser_error
{
	PARSER_SUCCESS, /**< Not an error: an ublox message was successfully parsed */
	PARSER_GARBAGE, /**< The first 2 characters read were not as expected, just try again */
	PARSER_ERROR_READ = 8,/**< Could not read enough characters */
	PARSER_ERROR_UNREACHABLE, /**< It should not be possible to emit this error */
};

static void queue_pop_into_array(queue_t* queue, array_t* array, size_t count)
{
	array_reserve(array, count);
	queue_pop_n(queue, array_end(array), count);
	array->count += count;
}

ublox_message_t* ublox_next_message(istream_t* input)
{
	queue_t* queue    = &input->buffer;
	array_t  result[] = {ArrayNew(uint8_t)};

sync:
	while (istream_accumulate(input, sizeof(ublox_sync_chars) + ublox_smallest_message_size)
	       && *(uint8_t*)queue_first(queue) != ublox_sync_chars[0])
	{
		queue_pop(queue, NULL);
	}
	if (ftq_is_empty(queue) || *(uint8_t*)ftq_first(queue) != ublox_sync_chars[0])
		goto fail;
	FTQ_POP_FRONT_ONE(queue, NULL);
	if (*(uint8_t*)ftq_first(queue) != ublox_sync_chars[1])
		goto sync;
	FTQ_POP_FRONT_ONE(queue, NULL);
	if (!is_valid_ublox_class(*(uint8_t*)ftq_first(queue)))
	{
		log_trace("Discaring header of unknown class %#.2hhx", *(uint8_t*)ftq_first(queue));
		goto sync;
	}
	ftq_pop_front_into_array(queue, result, sizeof(struct ublox_header));

	ublox_message_t** message = (ublox_message_t**)&result->data;
	if (!file_accumulate(file, message[0]->length + sizeof(struct ublox_footer)))
	{
		log_trace("Unable to read completely a message of class %s, discarding header",
		          cstring_from_ublox_class(message[0]->ublox_class));
		ft_print_memory(result->data, result->size);
		result->size = 0;
		goto sync;
	}
	ftq_pop_front_into_array(queue, result, message[0]->length);
	{
		ublox_checksum_t computed = ublox_compute_checksum(result->data, result->size);
		ublox_checksum_t expected;

		ftq_pop_front(queue, &expected, sizeof(expected));
		if (*(uint16_t*)&computed != *(uint16_t*)&expected)
		{
			RAII(t_string) str = ublox_header_tostring(message[0]);
			log_warn("Discarding message with invalid checksum: {%s}", cstring(&str));
			ft_print_memory(result->data, result->size);
			result->size = 0;
			goto sync;
		}
	}
	fta_trim(result);
	return result->data;
fail:
	fta_clear(result);
	return NULL;
}
