#include "ublox.h"
#include "ublox_enums.h"

#include <o2s/array.h> // array_t
#include <o2s/log.h>   // log_*

#include <inttypes.h> // PRIu16
#include <iso646.h>   // not
#include <stddef.h>   // size_t

static bool queue_pop_into_array(queue_t* queue, array_t* array, size_t count)
{
	if (queue->type_size != array->type_size
	    || queue_count(queue) < count
	    || not array_reserve(array, count)
	    || not queue_pop_n(queue, array_end(array), count))
		return false;
	array->count += count;
	return true;
}

/**
 * Try to parse the first bytes of _input_ as an ublox message.
 * Stops at the first error
 */
enum parser_error ublox_parse_single_message(istream_t* input, array_t* output)
{
	uint8_t           current;
	ublox_message_t** message = (ublox_message_t**)&output->start;
	ublox_checksum_t  computed;

	array_clear(output);
	if (not istream_accumulate(input, sizeof(ublox_sync_chars) + ublox_smallest_message_size))
		return PARSER_ERROR_READ;
	if (not queue_pop(&input->buffer, &current))
		return PARSER_ERROR_UNREACHABLE;
	if (current != ublox_sync_chars[0])
		return PARSER_GARBAGE;
	if (queue_count(&input->buffer) == 0)
		return PARSER_ERROR_UNREACHABLE;
	if (*(char*)queue_first(&input->buffer) != ublox_sync_chars[1])
		return PARSER_GARBAGE;
	if (not queue_pop(&input->buffer, NULL))
		return PARSER_ERROR_UNREACHABLE;
	if (!ublox_class_is_valid(*(uint8_t*)queue_first(&input->buffer)))
	{
		log_warning("Discarding message of unknown class %#.2hhx", *(uint8_t*)queue_first(&input->buffer));
		return PARSER_ERROR_INVALID_CLASS;
	}
	if (not queue_pop_into_array(&input->buffer, output, sizeof(struct ublox_header))
	    || array_count(output) != sizeof(struct ublox_header))
		return PARSER_ERROR_UNREACHABLE;
	if (not istream_accumulate(input, (*message)->length + sizeof(struct ublox_footer)))
	{
		log_warning("Unable to read completely a message of class %s and length %" PRIu16 ", discarding header",
		            ublox_class_to_cstring((*message)->ublox_class),
		            (*message)->length);
		return PARSER_ERROR_READ;
	}
	if (not queue_pop_into_array(&input->buffer, output, (*message)->length)
	    || queue_count(&input->buffer) < sizeof(struct ublox_footer))
		return PARSER_ERROR_UNREACHABLE;
	computed = ublox_compute_checksum(array_first(output), array_count(output));
	if (*(uint8_t*)queue_first(&input->buffer) != computed.a
	    || *(uint8_t*)queue_get(&input->buffer, 1) != computed.b)
		return PARSER_ERROR_WRONG_CHECKSUM;
	if (not queue_pop_n(&input->buffer, NULL, sizeof(struct ublox_footer)))
		return PARSER_ERROR_UNREACHABLE;
	return PARSER_SUCCESS;
}

/**
 * Places the first valid ublox message encountered in _output_.
 * Note that it will block until a valid message is received
 */
bool ublox_next_message(istream_t* input, array_t* output)
{
	enum parser_error status;

	do {
		status = ublox_parse_single_message(input, output);
		switch (status) {
		case PARSER_SUCCESS: return true;
		case PARSER_ERROR_WRONG_CHECKSUM:
			log_warning("Corrupted message discarded");
			break;
		case PARSER_ERROR_UNREACHABLE:
			log_fatal("The code is broken, stop using this version and report it to the devs !");
			break;
		}
	} while (status < PARSER_ERROR_READ);
	return false;
}

/** Similar to @ref ublox_next_message but will not block longer than _timeout_ms_ */
bool ublox_next_message_with_timeout(istream_t* input, array_t* output, o2s_timer_t* timer, unsigned timeout_ms)
{
	ArmedTimer armed = o2s_timer_start(*timer, timeout_ms);

	return ublox_next_message(input, output);
}
