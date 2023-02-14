#include "ublox.h"
#include "ublox_reader.h"

void ublox_subscribe(ublox_reader_t* reader, ublox_callback_t callback)
{
	fta_append(&reader->callbacks, &callback, 1);
}
