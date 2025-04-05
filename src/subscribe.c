#include "ublox.h"
#include "ublox_reader.h"

void ublox_subscribe(ublox_reader_t* reader, ublox_callback_t callback)
{
	array_push_back(&reader->callbacks, &callback);
}
