#pragma once

/**
 * @file file_input_stream.h
 */

#include <ft_deque.h>

#include <sys/types.h> // ssize_t

#include <stdbool.h>
#include <stdint.h> // uint*_t

/**  */
typedef struct file_input_stream
{
	t_deque buffer;     /**< Circular buffer */
	int     descriptor; /**< Underlying file */
	uint8_t opened :1;  /**< Is this file still open ? */
} ifstream_t;

ifstream_t    file_open(const char* file_name, int flags);
void          file_close(ifstream_t* file);

ssize_t       file_single_read(ifstream_t* file);
bool          file_accumulate(ifstream_t* file, size_t n);

/**
 * Use the RAII idiom with a file input stream.
 *
 * In a situation where one wants to declare an ifstream variable
 * on the stack in the local scope, this "typedef" can be used for
 * the file to be closed automatically when the variable goes out
 * of scope.
 *
 * It means this "typedef" can only be used like this :
 * @code{.c}
 * {
 *     FileInputStream file = file_open(...);
 *     ...
 * } // <- The file will be closed at that point
 * @endcode
 */
#define FileInputStream __attribute__((cleanup(file_close))) ifstream_t

/**
@var file_ipnut_stream::buffer
Good complexity for push_bash and pop_front.
No need to memmove remaining data to the front.
*/
