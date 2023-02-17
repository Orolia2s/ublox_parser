#include "file_input_stream.h"

#include <unistd.h> // read

/**
 * Read as much as possible.
 *
 * Attempt to fill the buffer
 */
ssize_t file_single_read(ifstream_t* file)
{
	t_deque* queue = &file->buffer;
	size_t   room  = ftq_capacity(queue) - ftq_size(queue);
	uint8_t  buffer[room];
	ssize_t  result = read(file->descriptor, buffer, room);

	if (result > 0)
		ftq_push_back(queue, buffer, result);
	return result;
}

/**
 * If possible, accumulate @p n bytes before returning
 * @return a boolean
 *  - `true` if n bytes were successfully accumulated
 *  - `false` otherwise
 */
bool file_accumulate(ifstream_t* file, size_t n)
{
	while (ftq_size(&file->buffer) < n)
		if (file_single_read(file) <= 0)
			return false;
	return true;
}
