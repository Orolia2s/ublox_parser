#include "ublox_reader.h"

struct ublox_reader ublox_reader_init(ifstream_t* input)
{
	return (struct ublox_reader){ .input = input, .callbacks = NEW_ARRAY(ublox_callback_t) };
}

void ublox_reader_close(ublox_reader_t* self)
{
	file_close(self->input);
	fta_clear(&self->callbacks);
}
