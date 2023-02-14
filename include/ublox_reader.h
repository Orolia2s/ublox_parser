#pragma once

#include "file_input_stream.h"
#include "ublox.h"

#include <ft_array.h>

/**
 * Pointer to a function that expects a ublox message
 */
typedef void (*ublox_callback_t)(ublox_message_t*);

typedef struct ublox_reader ublox_reader_t;

struct ublox_reader
{
	ifstream_t* input;
	t_array callbacks;
};

struct ublox_reader ublox_reader_init(ifstream_t* input);
void                ublox_subscribe(ublox_reader_t* reader, ublox_callback_t callback);
bool                ublox_reader_loop(ublox_reader_t* reader);
