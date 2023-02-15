#pragma once

/**
 * @file ublox_reader.h
 */

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
	ifstream_t* input; /**< Buffured input stream */
	t_array callbacks; /**<  */
};

struct ublox_reader ublox_reader_init(ifstream_t* input);
void                ublox_reader_close(ublox_reader_t* self);

void ublox_subscribe(ublox_reader_t* reader, ublox_callback_t callback);
bool ublox_reader_loop(ublox_reader_t* reader);

#define Reader ublox_reader_t __attribute__((cleanup(ublox_reader_close)))
