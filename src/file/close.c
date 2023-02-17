#include "file_input_stream.h"

#include <log.h>

#include <errno.h>
#include <stdlib.h> // free
#include <string.h> // strerror
#include <unistd.h> // close

/**
 * Release resources to the system
 *
 * In case of error when calling close, there is absolutely nothing interesting
 * that can be done beyond reporting it, so it is not propagated.
 */
void file_close(ifstream_t* file)
{
	log_trace("%s(%i)", __FUNCTION__, file->descriptor);
	if (file->buffer.data)
	{
		free(file->buffer.data);
		file->buffer.data = NULL;
		file->buffer.capacity = 0;
	}
	if (!file->opened)
	{
		log_warn("Attempting to close a file that isn't open.");
		return;
	}
	if (close(file->descriptor) != 0)
	{
		log_error("Unable to close the file: %s", strerror(errno));
		return;
	}
	file->opened = false;
}
