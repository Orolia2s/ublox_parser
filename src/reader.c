#include "ublox_reader.h"

struct ublox_reader ublox_reader_init(istream_t* input)
{
	return (struct ublox_reader){ .input = input, .callbacks = ArrayNew(ublox_callback_t), .timeout_ms = 1000 };
}

void ublox_reader_close(ublox_reader_t* self)
{
	array_clear(&self->callbacks);
}
