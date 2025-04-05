#pragma once

/**
 * @file ublox_reader.h
 */

#include "ublox.h"

#include <o2s/array.h>        // array_t
#include <o2s/input_stream.h> // istream_t

/**
 * Pointer to a function that expects a ublox message
 */
typedef void (*ublox_callback_t)(ublox_message_t*);

typedef struct ublox_reader ublox_reader_t;

struct ublox_reader
{
	istream_t* input; /**< Buffered input stream */
	array_t callbacks; /**< List of callbacks */
};

struct ublox_reader ublox_reader_init(istream_t* input);
void                ublox_reader_close(ublox_reader_t* self);

void ublox_subscribe(ublox_reader_t* reader, ublox_callback_t callback);
bool ublox_reader_loop(ublox_reader_t* reader);

/** RAII ublox_reader_t */
#define Reader ublox_reader_t __attribute__((cleanup(ublox_reader_close)))
