#include "file_input_stream.h"

#include <fcntl.h> // open
#include <log.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h> // malloc
#include <string.h> // strerror
#include <unistd.h> // close

#define BUFFER_SIZE 4096 // TODO: configure in a better way

/**
 * Construct a file input stream :
 * Open the file and allocate the buffer.
 */
ifstream_t file_open(const char* file_name, int flags)
{
	ifstream_t file = {.descriptor = -1, .opened = false};
	void*      buffer;

	log_trace("%s(\"%s\")", __FUNCTION__, file_name);
	if ((file.descriptor = open(file_name, flags)) < 0)
	{
		log_error("Unable to open \"%s\": %s", file_name, strerror(errno));
		return file;
	}
	if ((buffer = malloc(BUFFER_SIZE)) == NULL)
	{
		log_error("malloc failed: %s", strerror(errno));
		close(file.descriptor);
		file.descriptor = -1;
		return file;
	}
	file.opened = true;
	file.buffer = ftq_new(buffer, sizeof(char), BUFFER_SIZE);
	ftq_intent(&file.buffer, 'B'); // Will only push back
	return file;
}
